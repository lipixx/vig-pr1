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
  bool girant;
  float velocitat;
  
  
  //Ens indica si hem carregat el vehicle
  bool veh_carregat;

public:
  Vehicle();

  //FIXME
  float angle_direccio;
  float angle_direccio_realitzat;

  // carrega l'objecte
  void llegirModel (const char* filename); 

  // assigna la posici� del vehicle
  void setPos (Point);
  Point getPos();

  //assigna el tram en el que es troba el vehicle
  void setTramI (int);
  int getTramI();
  
  //Assigna el nou sentit de moviment del vehicle
  void setVelocitat(float v);
  float getVelocitat();

  //Assigna l'orientaci� del vehicle
  void setOrientation (float);
  float getOrientation();

  //Girant?
  void setGirant(bool);
  bool getGirant();
  
  //Heading!
  void setDireccio(float);
  void setDireccioRealitzat(float);
  float getDireccio();
  float getDireccioRealitzat();
  void addDireccioRealitzat(float);

  void Render ();
  bool enabled();
};

#endif
