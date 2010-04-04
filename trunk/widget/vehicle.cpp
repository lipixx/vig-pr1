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

   // Aqui cal que inicialitzeu correctament la resta d'atributs del vehicle
   Point pos(0,0,0);
   escalat = 1.0;
   orient = 0.0;
   veh_carregat = TRUE;
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
