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

#include <cmath>

//Per convertir a graus
#define PI 3.14159265
#define RAD2DEG 180/PI

class QDESIGNER_WIDGET_EXPORT GLWidget : public QGLWidget
{
  Q_OBJECT

 public:
  GLWidget(QWidget * parent);

 public slots:

  // help - Ajuda per la terminal des de la que hem  engegat el programa.
  void help(void);

  // Afegiu aquí la declaració dels slots que necessiteu

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
 
  void computeDefaultCamera(); 

  //Inicialitzem les matrius ModelView i Projection
  void initModelView();
  void initProjection();

 private:

  // interaccio
  typedef  enum {NONE, ROTATE, ZOOM, PAN} InteractiveAction;
  InteractiveAction DoingInteractive;
 
  int   xClick, yClick;
  
  Scene scene;  // Escena a representar en el widget

  //Atributs meus
  //Paràmetres càmera
  GLfloat fovy;
  //View Reference Point, és de la classe point i representa
  //un punt a l'espai.
  Point VRP;
  double distancia,radi,near,far;
  float angleX,angleY,angleZ,ratio;
  
};

#endif
