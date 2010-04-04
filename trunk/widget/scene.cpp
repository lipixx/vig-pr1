#include "scene.h"
#include "XML.h"
#include <math.h>

int Scene::DRETA=0;
int Scene::ESQUERRA=1;
int Scene::RECTA=2;
int Scene::ARBRE=3;
int Scene::PEDRES=4;
int Scene::CASA=5;
int Scene::DOBLE=6;
int Scene::TRIPLE=7;
int Scene::FANAL=8;

MaterialLib Scene::matlib;

Scene::Scene()
{}

void Scene::Init()
{
  XML xmlloader;
  xmlloader.LoadSceneDescription(this, "../data/escena.xml");
}


// Modifica un punt donat aplicant-li la transformació que es té guardada
// a la matriu MODELVIEW
void Scene::transformPoint(Point &p)
{
  GLfloat m[16];
  GLfloat p1[16];

  int i; 
  for (i=4; i<16; ++i) p1[i]=0.0f;

  p1[0] = p.x; 
  p1[1] = p.y; 
  p1[2] = p.z; 
  p1[3] = 1;

  glPushMatrix();
  glMultMatrixf(p1);
  glGetFloatv(GL_MODELVIEW_MATRIX, m);
  p.x = m[0];
  p.y = m[1];
  p.z = m[2];
  glPopMatrix();
}

// Mètode que pinta l'escena: tots els objectes i el vehicle
void Scene::Render()
{
  int num = lreferencies.size();

  // Cal pintar tots els objectes de l'escena que es troben als vectors
  // Cal pintar també el vehicle, si s'escau
  for (int i=0; i<num;i++) lreferencies[i].Render(lobjects);
  num = circuit.size();
  for (int i=0; i<num; i++) circuit[i].Render(lobjects);

  if (veh.enabled())
    
    veh.Render();
}

void Scene::AddObject(Object &o)
{
  lobjects.push_back(o);
}

void Scene::AddObject(Referencia &oref)
{
  lreferencies.push_back(oref);
}

void Scene::AddObject(Tram &tref)
{
  circuit.push_back(tref);
}

void Scene::CalculaEsfera (double &radi, Point &centreEscena)
{
  // creem la capsa inicialment amb els vèrtexs mínim i míxim del terra
  Box capsaEscena(Point (0.0, 0.0, 0.0), Point (0.0, 0.0, 0.0));
  Box bobj;
  // actualitzem la capsa amb tots els objectes de l'escena
  std::vector<Referencia>::iterator ito;
  for (ito=lreferencies.begin(); ito!=lreferencies.end(); ito++)
  {
    bobj=(*ito).computeTransformedBox(lobjects[(*ito).getObjectId()]);

    capsaEscena.update(bobj.minb);
    capsaEscena.update(bobj.maxb);
  }
  std::vector<Tram>::iterator itt;
  for (itt=circuit.begin(); itt!=circuit.end(); itt++)
  {
    bobj=(*itt).computeTransformedBox(lobjects[(*itt).getObjectId()]);

    capsaEscena.update(bobj.minb);
    capsaEscena.update(bobj.maxb);
  }
  centreEscena =  (capsaEscena.minb + capsaEscena.maxb)/2.;
  radi = (capsaEscena.maxb-capsaEscena.minb).length()/2.f;
}

void Scene::carregaVehicle(const char* filename)
{
  //Llegim el model
  veh.llegirModel(filename);

  //Obtenim la posició i orientació del tram amb Id 0.
  Point pos = circuit[0].getPosition();
  float ori = circuit[0].getOrientation();

  //Movem el vehicle a la posició del tram 0, i el pugem a nivell del terra
  //El nivell és 0.1, fer: cout << circuit[0].getSize() << endl;
  //També l'orientem correctament
  pos.y += 0.1;
  veh.setPos(pos);
  veh.setOrientation(180-ori);
}
