#include "glwidget.h"
#include <stdlib.h>
#define _USE_MATH_DEFINES 1
#include <cmath>

// Constructora amb format per defecte
GLWidget::GLWidget (QWidget * parent):QGLWidget (parent)
{
  // per rebre events de teclat
  setFocusPolicy (Qt::ClickFocus);
  connect (&timer, SIGNAL (timeout ()), this, SLOT (updateGL ()));
  timer.start (0);		//0ms Single-shot timer
  DoingInteractive = NONE;
}

// initializeGL() - Aqui incluim les inicialitzacions del contexte grafic.
void
GLWidget::initializeGL ()
{
  glClearColor (0.45f, 0.69f, 0.90f, 1.0f);
  glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
  glEnable (GL_DEPTH_TEST);

  glEnable (GL_NORMALIZE);
  glEnable (GL_LIGHTING);
  glEnable (GL_CULL_FACE);

  scene.Init ();
  computeDefaultCamera ();

  //Paràmetres 1ra persona
  far_pp = (float) radi *2;
  near_pp = scene.midaTram () * 0.1;
  fovy_pp = 90.0;
  ratio_pp = 1.;
  primera_persona = false;
  select = false;

  llums[0] = new llum (GL_LIGHT0, "Escena");
  llums[1] = new llum (GL_LIGHT1, "Vehicle");
  f_llums.iniciar (llums, 2);
  enableLlums ();
}

void
GLWidget::updateModelView ()
{
  // Inicialitza la matriu Modelview
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  if (!primera_persona)
    {
      //Posem l'escena a la distància que volguem, negatiu perqué s'allunyi de la càmera
      glTranslatef (0, 0, -distancia);

      //Rotem l'escena els angles que ens interessi, respecte el SCA
      glRotatef (angleZ, 0, 0, 1);
      glRotatef (angleX, 1, 0, 0);
      glRotatef (angleY, 0, 1, 0);

      //Centrem l'escena a l'origen de cordenades de l'SCA
      glTranslatef (-VRP.x, -VRP.y, -VRP.z);
    }
  else
    gluLookAt (posV.x, 0.5, posV.z, centreV.x, centreV.y, centreV.z, 0, 1, 0);
}

void
GLWidget::updateProjection ()
{
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  //Camera PERSPECTIVA, els objectes tornen grans i petits
  //segons la distància de la càmera
  if (!primera_persona)
    {
      if (ratio < 1)
	gluPerspective (dynamic_fovy, ratio, near, far);
      else
	gluPerspective (fovy, ratio, near, far);
    }
  else
    gluPerspective (fovy_pp, ratio_pp, near_pp, far_pp);
}

void
GLWidget::computeDefaultCamera ()
{
  /*Inicialitzem els paràmetres de la càmera inicial desitjada
     Calculem l'esfera contenidora. Es guarda el resultat dins radi i VRP.
     VRP és el centre de l'escena.
   */
  scene.CalculaEsfera (radi, VRP);

  distancia = 2 * radi;
  near = radi;
  far = 3 * radi;

  //Inicialment volem desplaçar l'escena
  //amb aquests angles perquè és com queda bé. (315=-45)
  angleX = 45.0;
  angleY = 340;
  angleZ = 0.0;

  /*
     Radi és perpendicular a la tangent que passa per l'esfera mínima.
     La tangent d'un angle de la càmera, és:
     tanα = catet oposat (radi) / distància 
     Llavors, necessitem trobar l'angle i multiplicar-ho per 2 quan el necessitem.
   */
  fovy = (float) 2 *atanf (radi / distancia) * RAD2DEG;
  dynamic_fovy = fovy;
  /*Ratio d'aspecte (16:9, 4:3..)
     width i height és la mida del viewport.
   */
  ratio = (float) width () / height ();
}

// paintGL() - callback cridat cada cop que cal refrescar la finestra. 
void
GLWidget::paintGL (void)
{
  if (!select)
    {
      centreV = scene.getVehOrientation ();
      posV = scene.getVehPos ();

      // Esborrem els buffers
      glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      updateModelView ();
      updateProjection ();

      // dibuixar eixos aplicacio
      glBegin (GL_LINES);
      glColor3f (1, 0, 0);
      glVertex3f (0, 0, 0);
      glVertex3f (20, 0, 0);	// X
      glColor3f (0, 1, 0);
      glVertex3f (0, 0, 0);
      glVertex3f (0, 20, 0);	// Y
      glColor3f (0, 0, 1);
      glVertex3f (0, 0, 0);
      glVertex3f (0, 0, 20);	// Z
      glEnd ();

      if (scene.getVehicle ())
	modificarLlumVehicle (llums[1]);
      // pintem l'escena
      scene.Render ();
    }
}

// resizeGL() - Cridat quan es canvia el tamany del viewport.
void
GLWidget::resizeGL (int width, int height)
{
  glViewport (0, 0, width, height);

  ratio = (float) width / height;
  if (ratio < 1)
    {
      dynamic_fovy = (atanf (tan (fovy * DEG2RAD / 2) / ratio)) * RAD2DEG * 2;
    }

  updateGL ();
}

// help() - Surt per la terminal des de la que hem 
// engegat el programa. En la versio amn interficie 
// hauria de crear una nova finestra amb la informacio...
void
GLWidget::help (void)
{
  cout << "Tecles definides: \n";
  cout << "f         Pinta en filferros\n";
  cout << "s         Pinta amb omplert de polígons\n";
  cout << "h,H,?     Imprimir aquesta ajuda\n";
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
void
GLWidget::keyPressEvent (QKeyEvent * e)
{

  switch (e->key ())
    {
    case Qt::Key_F:
      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
      updateGL ();
      break;
    case Qt::Key_S:
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      updateGL ();
      break;
    case Qt::Key_Space:
      aturaVehicle (scene.getRunning ());
      break;
    case 'h':
    case 'H':
    case '?':
      help ();
      break;
    default:
      e->ignore ();		// el propaguem cap al pare...
    }
}

void
GLWidget::wheelEvent (QWheelEvent * e)
{

  //Descomentar si volem que s'hagi de pitjar shift per fer zoom
  //amb la roda.
  //  if (e->modifiers()&Qt::ShiftModifier)
  {
    //apropa_allunya és un enter + si hem d'apropar, i - si hem
    //d'allunyar
    int apropa_allunya = -1 * e->delta () / 15 / 8;

    //Factor de zoom de 3.2
    float factor_zoom = apropa_allunya * 3.2;

    if (dynamic_fovy + factor_zoom > 0 && dynamic_fovy + factor_zoom < 180)
      {
	dynamic_fovy = dynamic_fovy + factor_zoom;
	fovy = dynamic_fovy;
      }
  }

  e->accept ();

  xClick = e->x ();
  yClick = e->y ();
  updateGL ();
}

/*--------------------
 * mousePressEvent()
 */
void
GLWidget::mousePressEvent (QMouseEvent * e)
{

  if (!select)
    {
      xClick = e->x ();
      yClick = e->y ();

      if (e->button () & Qt::LeftButton && e->modifiers () & Qt::ShiftModifier
	  && e->modifiers () & Qt::ControlModifier)
	{
	  //Copia d'objectes
	  DoingInteractive = SELECT;
	  select = true;
	  selectOn ();
	}

      if (e->button () & Qt::LeftButton &&
	  !(e->modifiers () &
	    (Qt::ShiftModifier | Qt::AltModifier | Qt::ControlModifier)))
	{
	  DoingInteractive = ROTATE;
	}
      else if (e->button () & Qt::LeftButton
	       && e->modifiers () & Qt::ShiftModifier)
	{
	  DoingInteractive = ZOOM;
	}
      else if (e->button () & Qt::RightButton
	       && e->modifiers () & Qt::ShiftModifier)
	{
	  DoingInteractive = PAN;
	}
    }
  else
    {
      if (e->button () & Qt::LeftButton &&
	  !(e->modifiers () &
	    (Qt::ShiftModifier | Qt::AltModifier | Qt::ControlModifier)))
	{
	  desactivarMoviment ();
	  select = false;
	}

      else if (e->button () & Qt::RightButton
	       && !(e->modifiers () &
		    (Qt::ShiftModifier | Qt::
		     AltModifier | Qt::ControlModifier)))
	{
	  desactivarMoviment ();
	  scene.selectCancel ();
	  select = false;
	}
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
void
GLWidget::mouseReleaseEvent (QMouseEvent *)
{
  if (!select)
    DoingInteractive = NONE;
  else
    DoingInteractive = SELECT;
}

/*--------------------
 *
 * mouseMoveEvent()
 *
 * Callback Qt cridat quan s'arrosega el ratoli amb
 * algun boto premut.
 *
 */
void
GLWidget::mouseMoveEvent (QMouseEvent * e)
{

  // Aquí cal que es calculi i s'apliqui la rotació, el zoom o el pan
  // com s'escaigui...

  if (!primera_persona)
    {
      int posX = e->x ();
      int posY = e->y ();

      if (DoingInteractive == ROTATE)
	{
	  // Fem la rotació
	  angleX += VEL_ROTACIO * (e->y () - yClick);
	  angleY -= VEL_ROTACIO * (xClick - (e->x ()));
	}

      else if (DoingInteractive == ZOOM)
	{
	  // Fem el zoom, dividim per obtenir més suavitat
	  if (dynamic_fovy + (e->y () - yClick) / 2 > 0
	      && dynamic_fovy + (e->y () - yClick) / 2 < 180)
	    {
	      dynamic_fovy = dynamic_fovy + (e->y () - yClick) / 2;
	      fovy = dynamic_fovy;
	    }
	}
      else if (DoingInteractive == PAN)
	{
	  float m[4][4];
	  glGetFloatv (GL_MODELVIEW_MATRIX, &m[0][0]);
	  Point x_obs =
	    Point (m[0][0], m[1][0], m[2][0]) * (xClick - e->x ());
	  Point y_obs =
	    Point (m[0][1], m[1][1], m[2][1]) * (e->y () - yClick);
	  VRP += (x_obs + y_obs) * VEL_PAN;	//Multipliquem per obtenir suavitat
	}

      else if (DoingInteractive == SELECT)
	{
	  //Sel·lecció i copia d'objectes
	  //Fem el select
	  glFlush ();
	  scene.pintarObj ();
	  Point x, y, p;

	  float m[4][4];
	  glGetFloatv (GL_MODELVIEW_MATRIX, &m[0][0]);

	  x.x = m[0][0];
	  x.y = m[1][0];
	  x.z = m[2][0];
	  x = x * 0.1;

	  y.x = m[1][0] * m[2][2] - m[1][2] * m[2][0];
	  y.y = m[1][0];
	  y.z = m[0][0] * m[1][2] - m[0][2] * m[1][0];
	  y = y * 0.2;

	  p = scene.getPosObj ();

	  //Actualitzar posicions de la copia
	  if (posX - xClick)
	    p += (x * (posX - xClick)) * 0.3;
	  if (posY - yClick)
	    p += (y * (posY - yClick)) * 0.3;

	  scene.setPosObjXZ (p);
	  scene.pintarObj ();
	  glFlush ();
	}
      xClick = e->x ();
      yClick = e->y ();
      updateGL ();
    }
}

void
GLWidget::resetCamera ()
{
  computeDefaultCamera ();
  updateGL ();
}

/********************************************VEHICLE*************************************************/

void
GLWidget::carregaVehicle ()
{
  QString fitxer =
    QFileDialog::getOpenFileName (this, tr ("Carregar vehicle"), "../data",
				  tr ("Objectes (*.obj)"));
  const char *veh = (fitxer.toStdString ()).c_str ();
  cout << "Carregem vehicle: " << veh << endl;
  scene.carregarVehicle (veh);
  emit setEnabled (true);
  updateGL ();
}

void
GLWidget::orientaVehicle (int graus)
{
  scene.orientaVehicle (-graus);
}

void
GLWidget::aturaVehicle (bool stop)
{
  scene.setRunning (!stop);
  emit aturat (!scene.getRunning ());
}

void
GLWidget::reiniciaVehicle ()
{
  scene.reiniciarV ();
}

void
GLWidget::setVelocitat (int velocitat)
{
  scene.setVelocitat (velocitat);
}

void
GLWidget::canviaCamera ()
{
  if (primera_persona)
    {				// de primera persona a tercera
      scene.setVisible (true);
      primera_persona = false;
    }
  else
    {				// de tercera persona a primera
      scene.setVisible (false);
      primera_persona = true;
    }
}

void
GLWidget::canviaPpFar (double f)
{
  if (far_pp < f)
    far_pp += (f - far_pp);

  if (far_pp > f)
    far_pp -= (far_pp - f);
}

/********************************************EFECTES I LLUMS*************************************************/

void
GLWidget::suavitzar (bool s)
{
  scene.suavitzar (s);
  scene.Render ();
  updateGL ();
}

void
GLWidget::enableLlums ()
{
  //Inicialitzar parametres inicials
  int i;
  float exponent;
  for (i = 0; i < 3; i++)
    ambient[i] = 0.0;
  ambient[3] = 1.0;
  for (i = 0; i < 3; i++)
    difosa[i] = 1.0;
  for (i = 0; i < 3; i++)
    especular[i] = 1.0;

  llums[0]->referencia = ESCENA;
  llums[1]->referencia = VEHICLE;

  //Per l'escena
  for (i = 0; i < 3; i++)
    {
      posicio[i] = 2 * radi;
      llums[0]->posicio[i] = posicio[i];
    }
  posicio[3] = 1.;
  llums[0]->posicio[3] = posicio[3];

  glLightfv (GL_LIGHT0, GL_POSITION, posicio);
  glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, difosa);
  glLightfv (GL_LIGHT0, GL_SPECULAR, especular);
  llums[0]->activada = true;
  glEnable (GL_LIGHT0);

  //Pel vehicle
  Point oriv = scene.orientLlum ();
  shineness = 60.0;
  exponent = 20.0;
  llums[1]->posicio[0] = posV.x;
  llums[1]->posicio[1] = 0.75;
  llums[1]->posicio[2] = posV.z;
  llums[1]->spotCutoff = 60;
  llums[1]->spotDireccio[0] = oriv.x;
  llums[1]->spotDireccio[1] = oriv.y;
  llums[1]->spotDireccio[2] = oriv.z;

  glLightfv (GL_LIGHT1, GL_AMBIENT, ambient);
  glLightfv (GL_LIGHT1, GL_DIFFUSE, difosa);
  glLightfv (GL_LIGHT1, GL_SPECULAR, especular);
  glLightfv (GL_LIGHT1, GL_SHININESS, &shineness);
  glLightfv (GL_LIGHT1, GL_POSITION, llums[1]->posicio);
  glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, llums[1]->spotDireccio);
  glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, llums[1]->spotCutoff);
  glLightf (GL_LIGHT1, GL_SPOT_EXPONENT, exponent);
  llums[1]->activada = false;

  //Millora els efectes de visualitzacio de l'escena
  glLightModelf (GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

  //Spots que usarem per modificar les llums des de l'interficie de llums
  connect (&f_llums, SIGNAL (llumModificada (llum *)), this,
	   SLOT (modificarLlumVehicle (llum *)));
  connect (&f_llums, SIGNAL (llumModificada (llum *)), this,
	   SLOT (modificarLlumEscena (llum *)));
}

//Mostra el widget.
void
GLWidget::finestraEditarLlums ()
{
  f_llums.show ();
}

void
GLWidget::enableLlumVehicle (bool act)
{
  if (scene.getVehicle ())
    {
      if (act && llums[1]->activada == false)
	{
	  llums[1]->activada = true;
	  glEnable (GL_LIGHT1);
	}
      else
	{
	  glDisable (GL_LIGHT1);
	  llums[1]->activada = false;
	}
    }
}

void
GLWidget::enableLlumEscena (bool act)
{

  if (act && llums[0]->activada == false)
    {
      llums[0]->activada = true;
      glEnable (GL_LIGHT0);
    }
  else
    {
      glDisable (GL_LIGHT0);
      llums[0]->activada = false;
    }
}

void
GLWidget::modificarLlumVehicle (llum * L)
{
  if (scene.getVehicle () && L->referencia == VEHICLE)
    {
      llums[1]->activada = L->activada;
      if (llums[1]->activada)
	{
	  emit activaLlumCotxe (true);
	}
      else
	{
	  emit activaLlumCotxe (false);
	}
      if (L->activada)
	{
	  llums[1]->posicio[0] = posV.x;
	  llums[1]->posicio[1] = 0.75;
	  llums[1]->posicio[2] = posV.z;

	  Point oriv = scene.orientLlum ();
	  llums[1]->spotDireccio[0] = oriv.x;
	  llums[1]->spotDireccio[1] = oriv.y;
	  llums[1]->spotDireccio[2] = oriv.z;

	  glLightfv (GL_LIGHT1, GL_DIFFUSE, L->difos);
	  glLightfv (GL_LIGHT1, GL_SPECULAR, L->especular);
	  glLightfv (GL_LIGHT1, GL_AMBIENT, L->ambient);
	  glLightfv (GL_LIGHT1, GL_SHININESS, &shineness);
	  glLightfv (GL_LIGHT1, GL_POSITION, L->posicio);
	  glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, L->spotDireccio);
	  glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, L->spotCutoff);
	  glLightf (GL_LIGHT1, GL_SPOT_EXPONENT, L->spotExponent);
	  glEnable (GL_LIGHT1);
	}
      else
	glDisable (GL_LIGHT1);
    }
}

void
GLWidget::modificarLlumEscena (llum * L)
{

  if (L->referencia == ESCENA)
    {
      llums[0]->activada = L->activada;

      if (llums[0]->activada)
	{
	  emit activaLlumEscena (true);
	}
      else
	{
	  emit activaLlumEscena (false);
	}
      if (L->activada)
	{
	  glLightfv (GL_LIGHT0, GL_DIFFUSE, L->difos);
	  glLightfv (GL_LIGHT0, GL_SPECULAR, L->especular);
	  glLightfv (GL_LIGHT0, GL_AMBIENT, L->ambient);
	  glLightfv (GL_LIGHT0, GL_POSITION, L->posicio);
	  glEnable (GL_LIGHT0);
	}
      else
	glDisable (GL_LIGHT0);
    }
}

/***********************************************COPIA D'OBJECTES***************************************************/

void
GLWidget::selectOn ()
{
 //Fa la còpia de l'objecte i l'afegeix al vector lreferencies
  int tam = scene.getReferencies ().size ();
  float ampl = 3.0;
  float alt = 3.0;
  GLuint hitBuffer[tam * 4], hits;
  GLint vp[4];

  glSelectBuffer (tam * 4, hitBuffer);

  glRenderMode (GL_SELECT);
  glGetIntegerv (GL_VIEWPORT, vp);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPickMatrix ((GLdouble) xClick, (GLdouble) vp[3] - yClick, ampl, alt, vp);
  float fov = (float) fovy;
  gluPerspective (fov, ratio, near, far);

  glInitNames ();
  scene.RenderSelect ();

  hits = glRenderMode (GL_RENDER);
  glEnable (GL_LIGHTING);
  //Si hem seleccionat algun objecte
  if (hits > 0)
    {
      //Cerquem l'objecte seleccionat
      name = selectName (hitBuffer, hits);
      
      //L'afegim a lreferencies
      scene.addObjIni (name);
      activarMoviment ();
      glMatrixMode (GL_PROJECTION);
      glLoadIdentity ();
      gluPerspective (fov, ratio, near, far);
      glFlush ();
      scene.pintarObj ();
      glFlush ();
    }
  else
    select = false;
}

int
GLWidget::selectName (GLuint * hitBuffer, GLuint hits)
{
  //Cercar l'objecte seleccionat
  GLuint min;
  uint i, name;

  min = hitBuffer[1];
  name = hitBuffer[3];

  for (i = 0; i < hits; i = i + 4)
    {
      if (min >= hitBuffer[i + 1])
	{
	  name = hitBuffer[i + 3];
	  min = hitBuffer[i + 1];
	}
    }
  return name;
}

void
GLWidget::activarMoviment ()
{
  //Anirem a moure l'objecte
  glDisable (GL_DEPTH_TEST);
  glDisable (GL_LIGHTING);
  setAutoBufferSwap (false);
  glEnable (GL_COLOR_LOGIC_OP);
  glLogicOp (GL_XOR);
  glDrawBuffer (GL_FRONT);
  glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
  //Moviment del ratolí sense estar clicat
  setMouseTracking (true);
  //Color de seleccio de color groc
  glColor3f (1., 1., 0.);
}

void
GLWidget::desactivarMoviment ()
{
  //Tornem l'escena en mode normal (sense select)
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_LIGHTING);
  setAutoBufferSwap (true);
  glDisable (GL_COLOR_LOGIC_OP);
  glDrawBuffer (GL_BACK);
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  setMouseTracking (false);
}
