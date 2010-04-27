#include "referencia.h"
#include "box.h"
#include <QtOpenGL/qgl.h>
#include "scene.h"

Referencia::Referencia(int ob, Point p, Vector sz, float ori):
  object(ob), pos(p), size(sz), orientation(ori)
{}

Referencia::~Referencia(void)
{}

void Referencia::Render(std::vector<Object> &lobjects, bool suavitzar)
{
  float sx, sy, sz;

  //Obtenim l'objecte d'aquesta referència
  Object o=lobjects[this->object];

  //Obtenim la capsa contenidora de l'objecte
  Box box=o.boundingBox();

  //Obtenim la mida de la capsa
  sx = box.maxb.x - box.minb.x;
  sy = box.maxb.y - box.minb.y;
  sz = box.maxb.z - box.minb.z;
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  
  //Movem l'objecte a la posició que li pertoca (del XML)
  glTranslatef(pos.x,pos.y,pos.z);

  //Rotem un angle "orientation" (del XML)  respecte l'eix y (la vertical).
  glRotatef(orientation,0.0,1.0,0.0);

  /*Definim la mida de l'obecte. El càlcul es fa escalant el size que té l'objecte
   que s'ha obtingut de l'XML, respecte la mida actual de la caixa contenidora
   de l'objecte. Recordem que la mida no té unitats, per això ho hem d'escalar
   respecte alguna cosa.*/
  glScalef(size.x/sx,size.y/sy,size.z/sz); 
  
  /*1. Obtenim el punt central de l'objecte respecte l'eix x:
    box.maxb.x+box.minb.x/2.0
    2. Igual per l'eix z:
    box.maxb.z+box.minb.z/2.0
    3. Tenint K, el punt central de l'eix x de l'objecte, movem l'objecte
    K posicions en l'eix x, obtindrem que el centre de l'eix x coincidirà
    amb l'origen.
    Fem el mateix per Z, i per Y només movem l'objecte cap amunt.
  */
  glTranslatef(-(box.maxb.x+box.minb.x)/2.0,-box.minb.y,-(box.maxb.z+box.minb.z)/2.0); 

  o.Render(suavitzar);
  //box.Render();
  glPopMatrix();

}

int Referencia::getObjectId()
{
  return object;
}

// Calculem la caixa transformada a partir de la caixa original
// de l'objecte.

Box& Referencia::computeTransformedBox(Object &obj)
{
  Box b = obj.boundingBox();
  Box *r=new Box(); // Retornem un Box, així que ens cal al heap.
  Box &result = *r;
  float sa, sb, sc;
  sa=b.maxb.x-b.minb.x;	
  sb=b.maxb.y-b.minb.y;	
  sc = b.maxb.z - b.minb.z;	
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glTranslatef(pos.x, pos.y, pos.z);
  glRotatef(orientation, 0, 1, 0);
  glScalef(size.x/sa, size.y/sb, size.z/sc);
  glTranslatef(-(b.maxb.x+b.minb.x)/2, -b.minb.y, -(b.maxb.z+b.minb.z)/2);

  vector<Vertex> &verts = obj.vertices;
  Point punt(verts[0].coord.x, verts[0].coord.y, verts[0].coord.z);
  Scene::transformPoint(punt);
  result.init(punt);

  for (unsigned int i=1; i<verts.size(); ++i)
  {
    Point punt(verts[i].coord.x, verts[i].coord.y, verts[i].coord.z);
    Scene::transformPoint(punt);
    result.update(punt);
  }
  glPopMatrix();
  return result;
}

Point Referencia::getPosition()
{
   return pos;
}

Vector Referencia::getSize()
{
   return size;
}

float Referencia::getOrientation()
{
   return orientation;
}

void Referencia::setPosition(Point p)
{
	pos = p;
}
