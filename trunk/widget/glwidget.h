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
#define PI 3.1415926535898
#define RAD2DEG 180/PI
#define DEG2RAD PI/180

class QDESIGNER_WIDGET_EXPORT GLWidget : public QGLWidget
{
  Q_OBJECT

 public:
  GLWidget(QWidget * parent);

 public slots:

  // help - Ajuda per la terminal des de la que hem  engegat el programa.
  void help(void);

  // Afegiu aquí la declaració dels slots que necessiteu
  void resetCamera();

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

  //Implementem zoom amb la roda del ratolí
  virtual void wheelEvent(QWheelEvent *e);
 
  void computeDefaultCamera(); 

  //Inicialitzem les matrius ModelView i Projection
  void updateModelView();
  void updateProjection();

 private:

  typedef enum {NONE, ROTATE, ZOOM, PAN} InteractiveAction;
  InteractiveAction DoingInteractive;
 
  int xClick, yClick;
  
  //Escena a representar en el widget
  Scene scene;
  
  //Paràmetres càmera
  GLfloat fovy,dynamic_fovy;
  Point VRP;
  double distancia,radi,near,far;
  float angleX,angleY,angleZ,ratio;
  
};

#endif
