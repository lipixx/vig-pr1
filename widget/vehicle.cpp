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

void Vehicle::Render ()
{
  // Cal aplicar les transformacions de model necessàries i pintar l'objecte

}

