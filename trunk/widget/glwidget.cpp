#include "glwidget.h"

#include <stdlib.h>
#define _USE_MATH_DEFINES 1
#include <cmath>

// Constructora amb format per defecte
GLWidget::GLWidget(QWidget * parent):QGLWidget(parent)
{
  // per rebre events de teclat
  setFocusPolicy(Qt::ClickFocus);
  
  DoingInteractive = NONE;  
}


// initializeGL() - Aqui incluim les inicialitzacions del contexte grafic.
void GLWidget::initializeGL()
{
  glClearColor(0.45f, 0.69f, 0.90f, 1.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_DEPTH_TEST);
  // inicialitzem també l'escena
  scene.Init();
  // dimensions escena i camera inicial
  computeDefaultCamera();
}

void GLWidget::initModelView()
{  
  // Inicialitza la matriu Modelview
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  //Posem l'escena a la distància que volguem, negatiu perqué s'allunyi de la càmera
  glTranslatef(0,0,-distancia);

  //Rotem l'escena els angles que ens interessi, respecte el SCA
  glRotatef(angleZ,0,0,1);
  glRotatef(angleX,1,0,0);
  glRotatef(angleY,0,1,0);

  //Centrem l'escena a l'origen de cordenades de l'SCA
  glTranslatef(-VRP.x,-VRP.y,-VRP.z);
}

void GLWidget::initProjection()
{    
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  //Camera PERSPECTIVA, els objectes tornen grans i petits
  //segons la distància de la càmera
  gluPerspective(fovy,ratio,near,far);
}

void GLWidget::computeDefaultCamera()
{
  /*Inicialitzem els paràmetres de la càmera inicial desitjada
  Calculem l'esfera contenidora. Es guarda el resultat dins radi i VRP.
  VRP és el centre de l'escena.
  */
  scene.CalculaEsfera(radi,VRP);

  distancia = 2*radi;
  near = radi;
  far = 3*radi;
  //Inicialment volem desplaçar l'escena
  //amb aquests angles perquè és com queda bé. (315=-45)
  angleX=40.0; 
  angleY=340.0;
  angleZ=0.0;
  /*
    Radi és perpendicular a la tangent que passa per l'esfera mínima.
    La tangent d'un angle de la càmera, és:
    tan α  = catet oposat (radi) / distància 
    Llavors, necessitem multiplicar-ho per 2.
  */
  fovy = (float) (2*tanf(radi/distancia))*RAD2DEG;

  /*Ratio d'aspecte (16:9, 4:3..)
    width i height és la mida del viewport.
   */
  ratio = (float) width()/height();
  
}

// paintGL() - callback cridat cada cop que cal refrescar la finestra. 
void GLWidget::paintGL( void )
{ 

  // Esborrem els buffers
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  // Init de les matrius
  initModelView();
  initProjection();
  
  // dibuixar eixos aplicacio
  glBegin(GL_LINES);
  
  glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(20,0,0); // X
  glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,20,0); // Y
  glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,20); // Z
  glEnd();
  
  // pintem l'escena
  scene.Render();
}

// resizeGL() - Cridat quan es canvia el tamany del viewport.
void GLWidget::resizeGL (int w, int h)
{
  glViewport (0, 0, w, h);
    
  //Recalculem objectiu de la càmera
  ratio = (float) w/h;

  if (ratio < 1) //Si w < h
    {
      /**A IMPLEMENTAR**/
      //float temp = fovy*DEG2RAD/2;
      //fovy = 2*fovy;
    }
  // cout << w <<":"<<h<<endl;
  updateGL();
}

// help() - Surt per la terminal des de la que hem 
// engegat el programa. En la versio amn interficie 
// hauria de crear una nova finestra amb la informacio...
void GLWidget::help( void ){
  cout<<"Tecles definides: \n";
  cout<<"f         Pinta en filferros\n";
  cout<<"s         Pinta amb omplert de polígons\n";
  cout<<"h,H,?     Imprimir aquesta ajuda\n";
  // Completar amb altres tecles que definiu...
  //
}

/*--------------------
*
*  teclat()
*
*  callback per quan hom prem una tecla.
*
*/
void GLWidget::keyPressEvent(QKeyEvent *e)
{
  switch( e->key() ) 
  {	    
    case Qt::Key_F: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      updateGL();
      break;
    case Qt::Key_S: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      updateGL();
      break;
    case  'h'  : case  'H'  : case  '?'  :  help();
      break;
    default: e->ignore(); // el propaguem cap al pare...
  }

}

/*--------------------
* mousePressEvent()
*/
void GLWidget::mousePressEvent( QMouseEvent *e){
  xClick = e->x();
  yClick = e->y();
  
  if (e->button()&Qt::LeftButton && 
      ! (e->modifiers()&(Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
  else if (e->button()&Qt::LeftButton &&  e->modifiers() &Qt::ShiftModifier)
  {
    DoingInteractive = ZOOM;
  }
  else if (e->button()&Qt::LeftButton &&  e->modifiers() &Qt::ControlModifier)
  {
    DoingInteractive = PAN;
  }
}

/*--------------------
*
* mouseReleaseEvent()
*
* Callback Qt cridat quan es deixa anar el botó del
* ratolí.
*
*/
void GLWidget::mouseReleaseEvent( QMouseEvent *){
  DoingInteractive = NONE;
}

/*--------------------
*
* mouseMoveEvent()
*
* Callback Qt cridat quan s'arrosega el ratoli amb
* algun boto premut.
*
*/
void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
  // Aquí cal que es calculi i s'apliqui la rotació, el zoom o el pan
  // com s'escaigui...
  
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
  }
  else if (DoingInteractive == ZOOM)
  {
    // Fem el zoom
  }
  else if (DoingInteractive==PAN)
  {  
    // Fem el pan
  }   
  xClick = e->x();
  yClick = e->y();
  
  updateGL();
}

