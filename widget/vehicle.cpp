#include "vehicle.h"
#include "scene.h"

#define _USE_MATH_DEFINES 1
#include <cmath>

using namespace std;

Vehicle::Vehicle():obj("VEHICLE"), orient(0)
{
  veh_carregat = FALSE;
}

void Vehicle::llegirModel (const char* filename)
{
   obj.readObj(filename, Scene::matlib);
   obj.updateBoundingBox();
   Box box=obj.boundingBox();
   float sx = box.maxb.x - box.minb.x;
   float sy = box.maxb.y - box.minb.y;
   float sz = box.maxb.z - box.minb.z;
   float max;

   max = sz;
   if (max < sy) max = sy;
   if (max < sx) max = sx;
     
   // Aqui cal que inicialitzeu correctament la resta d'atributs del vehicle
   Point pos(0,0,0);

   //Ha de ser la meitat de la mida d'un tram
   //mirant el fitxer scene.xml: (1,0.1,1). Ens interessa
   //la mida de X i de Y que defineix el quadrat. És 1.0.
   escalat = 1.0/(2.0*max)*1.0;

   veh_carregat = TRUE;
}

void Vehicle::setDireccio(float angledireccio)
{
  angle_direccio = angledireccio;
}

float Vehicle::getDireccio()
{
  return angle_direccio;
}

void Vehicle::setDireccioRealitzat(float angledireccio)
{
  angle_direccio_realitzat = angledireccio;
}

void Vehicle::addDireccioRealitzat(float angledireccio)
{
  angle_direccio_realitzat += angledireccio;
}

float Vehicle::getDireccioRealitzat()
{
  return angle_direccio_realitzat;
}

bool Vehicle::getGirant()
{
  return girant;
}

void Vehicle::setGirant(bool b)
{
  girant = b;
}

float Vehicle::getVelocitat()
{
  return velocitat;
}

void Vehicle::setVelocitat(float v)
{
  velocitat = v/(float)100;
}


void Vehicle::setTramI (int nouTramI)
{
  indexTram  = nouTramI;
}

int Vehicle::getTramI()
{
  return indexTram;
}

void Vehicle::setPos (Point p)
{
  pos=p;
}

Point Vehicle::getPos()
{
   return pos;
}

void Vehicle::setOrientation(float o)
{
   orient=o;
}

float Vehicle::getOrientation()
{ 
   return orient;
}

bool Vehicle::enabled()
{
  return veh_carregat;
}

void Vehicle::Render ()
{
  Box box=obj.boundingBox();

  glMatrixMode(GL_MODELVIEW); 
  glPushMatrix();

  //Traslladar l'objecte a on volem
  glTranslatef(pos.x, pos.y, pos.z);
   
  //Orientar l'objecte respecte y
  glRotatef(orient,0.0,1.0,0.0);
  
  //Escalar l'objecte
  glScalef(escalat,escalat,escalat);
  
  //Portar l'objecte a l'origen de coordenades
  glTranslatef(
	       -(box.maxb.x+box.minb.x)/2.0,
	       -box.minb.y,
	       -(box.maxb.z+box.minb.z)/2.0);

  obj.Render();
  //box.Render(); //Debugging
  glPopMatrix();
}
