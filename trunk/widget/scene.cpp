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
  veh.setTramI(0);
  veh.setOrientation(180-ori);
}

void Scene::orientaVehicle(int graus)
{
  veh.setOrientation((float)graus);
}

void Scene::mouVehicle()
{
  Point veh_pos; //Posició actual del vehicle
  Point mov,tmp;
  int seguent_tram;
  int seg_direccio;
  double xmov,zmov;

  tmp = veh.getMov();
  
  xmov = tmp.x;
  zmov = tmp.z;
  
  veh_pos = veh.getPos();
  mov = Point(xmov,0,zmov);

  //Movem el vehicle
  veh_pos += mov;
  veh.setPos(veh_pos);

  if (veh.getGirant())
    {
      float v_ori,t_ori;
      v_ori = veh.getOrientation();
      t_ori = circuit[veh.getTramI()].getOrientation();
      if (v_ori < t_ori)
	veh.setOrientation(v_ori+20);
      else
	veh.setOrientation(v_ori-20);
    }

  //En el seguent moviment, on estarem?
  seguent_tram = vehInTram(veh_pos,veh.getTramI());

  //Si estic al mateix tram, no fer res
  //si estic a un tram nou, identificar en quin,
  //actualitzant veh.indexTram.
  //Decidir un nou sentit de moviment en funció
  //d'aquest (veh xmov,zmov).
  if (seguent_tram != veh.getTramI())
    {
      veh.setTramI(seguent_tram);
      
      seg_direccio = circuit[seguent_tram].seguentDireccio();
      
      switch (seg_direccio)
	{
	case XPOS:
	  if (xmov != VELOCITAT)
	    {
	      xmov = VELOCITAT;
	      zmov = 0;
	      veh.setGirant(true);
	    }
	  break;
	case XNEG:
	  if (xmov != -VELOCITAT)
	    {
	      xmov = -VELOCITAT;
	      zmov = 0;
	      veh.setGirant(true);
	    }
	  break;
	case ZPOS:
	  if (zmov != -VELOCITAT)
	    {    
	      xmov = 0;
	      zmov = VELOCITAT;
	      veh.setGirant(true);
	    }
	  break;
	case ZNEG:
	  if (zmov != -VELOCITAT)
	    {
	      xmov = 0;
	      zmov = -VELOCITAT;
	      veh.setGirant(true);
	    }
	  break;
        default:
	  break;
	}
      veh.setMov(xmov,zmov);
    }
}

int Scene::vehInTram(Point veh_pos,int index_tram)
{
  double sx,sz,snx,snz;
  Point vmov,ptram;
  
  ptram = circuit[index_tram].getPosition();

  //Mirem si estem a dins del ptram (de mida 1)
  sx = ptram.x + 1;
  sz = ptram.z + 1;
  snx = ptram.x - 1;
  snz = ptram.z - 1;

  if (veh_pos.x > sx || veh_pos.x < snx ||
      veh_pos.z > sz || veh_pos.z < snz)
    {
      //Hem caigut fora de del tram

      //Cap on anavem?
      vmov = veh.getMov();

      if (vmov.x > 0)
	{
	  //Ens moviem en l'eix x +
	  return circuit[index_tram].getSeg(XPOS);
	}
      if (vmov.z > 0)
	{
	  //Ens moviem en l'eix z +
	  return circuit[index_tram].getSeg(ZPOS);
	}
      if (vmov.x < 0)
	{
	  //Ens moviem en l'eix x -
	  return circuit[index_tram].getSeg(XNEG);
	}
      if (vmov.z < 0)
	{
	  //Ens moviem en l'eix z -
	  return circuit[index_tram].getSeg(ZNEG);
	}
    }
  return index_tram;
}
