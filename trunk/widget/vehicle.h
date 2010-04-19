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
  float orient;        // orientaci� (en graus) respecte l'eix Y

  //Moviment
  int indexTram;
  double xmov,zmov; //Direcció en la que ens estem movent
  bool girant;

  //Ens indica si hem carregat el vehicle
  bool veh_carregat;

public:
  Vehicle();

  // carrega l'objecte
  void llegirModel (const char* filename); 

  // assigna la posici� del vehicle
  void setPos (Point);
  Point getPos();

  //assigna el tram en el que es troba el vehicle
  void setTramI (int);
  int getTramI();
  
  //Assigna el nou sentit de moviment del vehicle
  void setMov(double xmov,double zmov);
  Point getMov();

  //Assigna l'orientaci� del vehicle
  void setOrientation (float);
  float getOrientation();

  //Girant?
  void setGirant(bool);
  bool getGirant();

  void Render ();

  bool enabled();
};

#endif
