#include "referencia.h"
#include "box.h"
#include <QtOpenGL/qgl.h>
#include "scene.h"

Referencia::Referencia(int ob, Point p, Vector sz, float ori):
  object(ob), pos(p), size(sz), orientation(ori)
{}

Referencia::~Referencia(void)
{}

void Referencia::Render(std::vector<Object> &lobjects)
{
  float sx, sy, sz;

  Object o=lobjects[this->object];
  Box box=o.boundingBox();

  // A partir d'aqu�, cal aplicar les transformacions de model necess�ries 
  // i pintar l'objecte

  sx = box.maxb.x - box.minb.x;
  sy = box.maxb.y - box.minb.y;
  sz = box.maxb.z - box.minb.z;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  
  glTranslatef(pos.x,pos.y,pos.z);
  glRotatef(orientation,0.0,1.0,0.0);
  glScalef(size.x/sx,size.y/sy,size.z/sz);
  glTranslatef(-(box.maxb.x+box.minb.x)/2.0,-box.minb.y,-(box.maxb.z+box.minb.z)/2.0);
  o.Render();
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
  Box *r=new Box(); // Retornem un Box, aix� que ens cal al heap.
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
