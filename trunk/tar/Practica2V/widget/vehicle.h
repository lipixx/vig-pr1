#ifndef _Vehicle_H_
#define _Vehicle_H_
#include "object.h"
#include <QtOpenGL/qgl.h>

class Vehicle
{
  friend class Scene;

 private:
  Object obj;
  Point pos;         // posici� del centre de la base del vehicle
  double escalat;    // escalat (homogeni) que cal aplicar-li en pintar-lo
  int orient;        // orientaci� (en graus) respecte l'eix Y
  float velocitat;
  float vCurva;

public:
  Vehicle();

  // carrega l'objecte
  void llegirModel (const char* filename); 

  // assigna la posici� del vehicle
  void setPos (Point);
  Point getPos();
 
  // assigna l'orientaci� del vehicle
  void setOrientation (int);
  int getOrientation();

  void Render ();

  float getVelocitat();
  void setVelocitat(int v);
  float getVCurva();
  
};

#endif
