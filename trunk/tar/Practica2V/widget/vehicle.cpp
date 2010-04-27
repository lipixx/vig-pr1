#include "vehicle.h"
#include "scene.h"

#define _USE_MATH_DEFINES 1
#include <cmath>

using namespace std;

Vehicle::Vehicle():obj("VEHICLE"), orient(0)
{}

void Vehicle::llegirModel (const char* filename)
{
   obj.readObj(filename, Scene::matlib);
   obj.updateBoundingBox();
   Box box=obj.boundingBox();

   // Aqui cal que inicialitzeu correctament la resta d'atributs del vehicle

   // pos l'inicialitzem a l'escena
  float sx = box.maxb.x - box.minb.x;
   float sy = box.maxb.y - box.minb.y;
   float sz = box.maxb.z - box.minb.z;
   float max = sz;
   if(max < sx) max = sx;
   if(max < sy) max = sy;

   escalat = 1.0/(2.0*max);
   
   //escalat = 1; // AIXÒ S'HA DE MODIFICAR!!!

   orient = 0;

   velocitat = 0.01;
   
   vCurva = 1;

}

void Vehicle::setPos (Point p)
{
  pos=p;
}

Point Vehicle::getPos()
{
   return pos;
}

void Vehicle::setOrientation(int o)
{
   orient=o;
}

int Vehicle::getOrientation()
{ 
   return orient;
}

void Vehicle::Render ()
{
  // Cal aplicar les transformacions de model necessàries i pintar l'objecte

  // Utilitzem el mateix render que en les referencies
 
  Box box=obj.boundingBox();
  
  glMatrixMode(GL_MODELVIEW); 
  glPushMatrix();
  
  // Cal tenir en compte que s'apliquen les instruccions en ordre invers:
  
  // 4t. col·loquem l'objecte en la posició on s'ha de pintar
  glTranslatef(pos.x, pos.y, pos.z);
   
  // 3r. feim les rotacions oportunes segons els paràmetre: orient (respecte l'eix Y)
  glRotatef(orient,0.0,1.0,0.0);
  
  // 2n. escalem l'objecte de forma homogenia segons el paràmetre: escalat
  glScalef(escalat,escalat,escalat);
  
  // 1r. col·loquem l'objecte a l'eix de coordenades per poder fer totes les transformacions oportunes
  // Necessitem calcular el centre de la capsa: (max + min)/2  
  // per a l'eix Y, agafem la posició mínima perquè pos representa la posició sobre el terra!
  glTranslatef(-(box.maxb.x+box.minb.x)/2.0,-box.minb.y,-(box.maxb.z+box.minb.z)/2.0);
  
  // Ara ja podem pintar l'objecte
  obj.Render(false);
  
  glPopMatrix();
}

float Vehicle::getVelocitat()
{
  return velocitat;
}

void Vehicle::setVelocitat(int v)
{
  switch(v){
  case 4: v=3;
  case 7: v=6;
  case 8: v=9;
  }
  velocitat = (float)v*0.01;
    
  vCurva = (float) velocitat*100;
}

float Vehicle::getVCurva()
{
  return vCurva;
}
