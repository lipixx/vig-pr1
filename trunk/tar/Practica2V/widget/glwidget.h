#ifndef _GLWIDGET_H_
#define _GLWIDGET_H_

#include <QtOpenGL/qgl.h>
#include <QKeyEvent>
#include <iostream>
#include <qstring.h>
#include <qfiledialog.h>
#include <qtimer.h>
#include <QtDesigner/QDesignerExportWidget>

#include "material_lib.h"
#include "point.h"
#include "scene.h"

#include "uiLlums/finestraLlums.h"
#include "uiLlums/llum.h"

#define VEL_ROTACIO 0.6
#define VEL_PAN 0.05

class QDESIGNER_WIDGET_EXPORT GLWidget : public QGLWidget
{
  Q_OBJECT

 public:
  GLWidget(QWidget * parent);

 public slots:

  // help - Ajuda per la terminal des de la que hem  engegat el programa.
  void help(void);

  // Afegiu aquí la declaració dels slots que necessiteu

  //Vehicle
  void carregaVehicle();
  void aturaVehicle(bool stop);
  void reiniciaVehicle();
  void orientaVehicle(int graus);
  void setVelocitat(int v);
  //Càmera
  void resetCamera();
  void canviaCamera();
  void canviaPpFar(double f);
  //Efectes
  void suavitzar(bool s);
  void finestraEditarLlums();
  void enableLlumVehicle(bool act);
  void enableLlumEscena(bool act);
  void modificarLlumVehicle(llum *L);
  void modificarLlumEscena(llum *L);

 protected:
  // initializeGL() - Aqui incluim les inicialitzacions del contexte grafic.
  virtual void initializeGL();

  // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
  // Tot el que es dibuixa es dibuixa aqui.
  virtual void paintGL( void );
 
  // resizeGL() - Es cridat quan canvi la mida del widget
  virtual void resizeGL (int width, int height);

  // keyPressEvent() - Cridat quan es prem una tecla
  virtual void keyPressEvent(QKeyEvent *e);

  // mousePressEvent() i mouseReleaseEvent()
  virtual void mousePressEvent( QMouseEvent *e);
  virtual void mouseReleaseEvent( QMouseEvent *);
 
  // mouseMoveEvent() - Cridat quan s'arrosega el ratoli amb algun botó premut.
  virtual void mouseMoveEvent(QMouseEvent *e);
  
  // Implementem zoom amb la roda del ratolí
  virtual void wheelEvent(QWheelEvent *e);

  void computeDefaultCamera(); 

  //Inicialitzem les matrius ModelView i Projection
  void updateModelView();
  void updateProjection();

 signals:

  void aturat(bool check);
  void setEnabled(bool enable);

  //Encendre/apagar llum escena/cotxe, controlat per la finestra de llums
  void activaLlumEscena(bool b);
  void activaLlumCotxe(bool b);
 
 private:

  typedef  enum {NONE, ROTATE, ZOOM, PAN, SELECT} InteractiveAction;
  InteractiveAction DoingInteractive;
 
  int   xClick, yClick, name;
  
  //Escena a representar en el widget
  Scene scene;  
  QTimer timer;
  Point posV, centreV;

  //Paràmetres de la camera
  GLfloat fovy, dynamic_fovy, fovy_pp;
  double near, far, near_pp, far_pp;
  double distancia, radi; 
  float angleX, angleY, angleZ, ratio, ratio_pp;
  Point VRP;
  bool primera_persona;

  //Llums
  finestraLlums f_llums;
  llum* llums[2];
  GLfloat ambient[4];
  GLfloat difosa[4];  
  GLfloat especular[4]; 
  GLfloat shineness;
  GLfloat posicio[4];
  void enableLlums();

  //Select
  bool select;
  Point posObj;
  GLdouble ampl, alt;

  //Copia de l'objecte i l'afegeix al vector lreferencies 
  void selectOn();
  //Cerca de l'objecte seleccionat
  int selectName (GLuint *hitBuffer, GLuint hits);
  //Preparació de l'escena per moure l'objecte
  void activarMoviment();
  //Switch de l'escena a mode normal
  void desactivarMoviment();
};

#endif
