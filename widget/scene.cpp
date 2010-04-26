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
  veh.setVelocitat(1.5);
  veh.setOrientation(ori);
  veh.setGirant(false);
  veh.setDireccio((float)ori);
  veh.setDireccioRealitzat(ori);
}

void Scene::orientaVehicle(int graus)
{
  veh.setOrientation((float)graus);
}

void Scene::mouVehicle()
{

  Point veh_pos,mov;
  int seguent_tram;
  float v_ori;

  v_ori = veh.getDireccio();

  veh_pos = veh.getPos();

  if (!veh.getGirant())
    {
      mov = getNextMov(v_ori);
      veh.setPos(mov+veh_pos);
    }
  else
    {
      //Hem de veure cap on girar, si dreta, esquerra dalt o baix.
     
      float heading_tram = circuit[veh.getTramI()].getOrientation();
      float desti_veh = veh.getDireccio(); 

      mov.x = 0;
      mov.y = 0.1;
      mov.z = 0;
      // float radi_gir = 0.5;
      float angle_realitzat = veh.getDireccioRealitzat();
      Point pgir = veh.getPuntGir();

      if (heading_tram == 180)
	{
	  switch ((int) desti_veh)
	    {
	    case 90:
	      {
		veh.addDireccioRealitzat(-1);
		//Anem X=180 i girem a ZPOS, fletxa dreta.
		mov.x = pgir.x + (cos(angle_realitzat*DEG2RAD))/2;
		mov.z = pgir.z + (1-sin(angle_realitzat*DEG2RAD))/2;
		if (angle_realitzat == 0)
		  veh.setGirant(false);
		break;
	      }
	    case 270:
	      {
		veh.addDireccioRealitzat(1);
		//Anem X=180 i girem a ZPOS, fletxa dreta.
		mov.x = pgir.x + (cos(angle_realitzat*DEG2RAD))/2;
		mov.z = pgir.z - (1-sin(angle_realitzat*DEG2RAD))/2;
		break;
	      }
	    }
	}
      else
	if (heading_tram == 0)
	  {
	    switch ((int) desti_veh)
	      {
	      case 90:
		{
		  veh.addDireccioRealitzat(1);
		  mov.x = pgir.x + (cos(angle_realitzat*DEG2RAD))/2;
		  mov.z = pgir.z + (1-sin(angle_realitzat*DEG2RAD))/2;
		  break;
		}
	      case 270:
		{
		  veh.addDireccioRealitzat(-1);
		  mov.x = pgir.x + (cos(angle_realitzat*DEG2RAD))/2;
		  mov.z = pgir.z + (1-sin(angle_realitzat*DEG2RAD))/2;
		  break;
		}		
	      default:
		break;
	      }
	  }
      else
	if (heading_tram == 270)
	  {
	    switch ((int) desti_veh)
	      {
	      case 0:
		{
		  veh.addDireccioRealitzat(1);
		  mov.x = pgir.x + (cos(angle_realitzat*DEG2RAD))/2;
		  mov.z = pgir.z + (1-sin(angle_realitzat*DEG2RAD))/2;
		  break;
		}
	      case 180:
		{
		  veh.addDireccioRealitzat(-1);
		  mov.x = pgir.x + (cos(angle_realitzat*DEG2RAD))/2;
		  mov.z = pgir.z + (1-sin(angle_realitzat*DEG2RAD))/2;
		  break;
		}
	      default:
		break;
	      }
	  }
	else
	  if (heading_tram == 90)
	    {
	      switch ((int) desti_veh)
		{
		case 0:
		  {
		    cout <<"0:" <<angle_realitzat << endl;
		    veh.addDireccioRealitzat(-1);
		    mov.x = pgir.x + (cos(angle_realitzat*DEG2RAD))/2;
		    mov.z = pgir.z + (sin(angle_realitzat*DEG2RAD))/2;
		    if (angle_realitzat == 0)
		      veh.setGirant(false);	    
		    break;
		  }
		case 180:
		  {
		    cout << angle_realitzat << endl;
		    veh.addDireccioRealitzat(1);
		    mov.x = pgir.x + (1-cos(angle_realitzat*DEG2RAD))/2;
		    mov.z = pgir.z + (1-sin(angle_realitzat*DEG2RAD))/2;		    
		    if (angle_realitzat == 180)
		      veh.setGirant(false);
		    break;
		  }
		default:
		  break;
		}
	    }

      veh.setPos(mov);
    }
  
  //En el seguent moviment, on estarem?
  seguent_tram = vehInTram(veh.getPos(),veh.getTramI());
  
  //Si estic al mateix tram, no fer res
  //si estic a un tram nou, identificar en quin,
  //actualitzant veh.indexTram.
  //Decidir un nou sentit de moviment en funció
  //d'aquest (veh xmov,zmov).
  if (seguent_tram != veh.getTramI())
    {
      veh.setTramI(seguent_tram);

      //Hem de veure on ens portarà aquest tram, per veure si s'ha 
      //de fer gir o no
      int dir = circuit[seguent_tram].seguentDireccio();

      if (veh.getDireccio() != dir)
	{
	  //Haurem d'iniciar sa giravoga
	  veh.setGirant(true);

	  //Punt inicial de gir
	  veh.setPuntGir(veh.getPos());

	  //Girarem a poc a poc, a partir de 0 graus a 90
	  //fins al nou angle
	  if (circuit[seguent_tram].getOrientation() == 180)
	    switch(dir)
	      {
	      case 90:
		veh.setDireccioRealitzat(90);
		break;
	      case 270:
		veh.setDireccioRealitzat(270);
		break;
	      }
	  
	  else
	    if (circuit[seguent_tram].getOrientation() == 270)
	      switch (dir)
		{
		case 0:
		  veh.setDireccioRealitzat(180);
		  break;
		case 180:
		  veh.setDireccioRealitzat(360);
		  break;
		}
	    else
	      if (circuit[seguent_tram].getOrientation() == 0)
	      switch (dir)
		{
		case 90:
		  veh.setDireccioRealitzat(270);
		  break;
		case 270:
		  veh.setDireccioRealitzat(90);
		  break;
		}
	      else
		if (circuit[seguent_tram].getOrientation() == 90)
		  switch (dir)
		    {
		    case 0:
		      veh.setDireccioRealitzat(90);
		      break;
		    case 180:
		      veh.setDireccioRealitzat(90);
		      break;
		    }

	  //I a partir d'ara ens dirigirem cap aquí
	  veh.setDireccio((float)dir);
	}
    }
}

Point Scene::getNextMov(float v_ori)
{
  Point mov;
  float vel = veh.getVelocitat();

  mov.x = 0;
  mov.y = 0;
  mov.z = 0;

  if (v_ori == 180) { mov.x = vel; mov.z = 0; }
  if (v_ori == 90) { mov.x = 0;  mov.z = vel; }
  if (v_ori == 0) { mov.x = -vel; mov.z = 0; }
  if (v_ori == 270) { mov.x = 0; mov.z = -vel; }

  return mov;
}

float Scene::vehInTram(Point veh_pos,int index_tram_anterior)
{
  //Aquesta funció retorna la direccio cap on ha d'anar el vehicle
  //a partir d'aquest tram.

  float sx,sz,snx,snz;
  Point ptram;
  // float angle_direccio;

  ptram = circuit[index_tram_anterior].getPosition();

  //Mirem si estem a dins del ptram (de mida 1)
  sx = ptram.x + 0.5;
  sz = ptram.z + 0.5;
  snx = ptram.x - 0.5;
  snz = ptram.z - 0.5;

  if (veh_pos.x > sx || veh_pos.x < snx ||
      veh_pos.z > sz || veh_pos.z < snz)
    {
      //Hem caigut fora de del tram
      //Ens moviem en l'eix z -

      if (veh.getDireccio() == 180)
	return circuit[index_tram_anterior].getSeg(XPOS);
      if (veh.getDireccio() == 0)
	return circuit[index_tram_anterior].getSeg(XNEG);
      if (veh.getDireccio() == 90)
	return circuit[index_tram_anterior].getSeg(ZPOS);
      if (veh.getDireccio() == 270)
	return circuit[index_tram_anterior].getSeg(ZNEG);
    }
  
  return index_tram_anterior;
}
