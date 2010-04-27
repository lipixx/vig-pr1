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
  vehicle = false;
  engegat = false;
  tram=0;
  dif_ori=0;
  /*
   * 180 : XPOS
   * 90 : ZPOS
   * 270: ZNEG
   * 0 : XNEG
   */
  orientacio_anterior = 180; 
  direccio = 2; // inicialment anem en linia recta
  velocitat=0;
  suavizat = false;
  tram_ori = 0;
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
  for (int i=0; i<num;i++) lreferencies[i].Render(lobjects,suavizat);
  num = circuit.size();
  for (int i=0; i<num; i++) circuit[i].Render(lobjects,false);
  
  if(vehicle) veh.Render();
  if(engegat)moureVehicle();
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
  // creem la capsa inicialment amb els vèrtexs mínim i màxim del terra
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

void Scene::carregarVehicle(const char * filename)
{
 //Llegim el model
  veh.llegirModel(filename);
  tram_ori = circuit[0].getOrientation();
  orientacio_inicial=veh.getOrientation();
  vehicle = true;
  engegat = true;
  //Inicialitzem valors per defecte
  reiniciarV(); 
}

void Scene::moureVehicle()
{
  switch(circuit[tram].getObjectId())
    {
    /*En els casos 0, 1 i 6 es fa girar(). (Fletxa dreta, esquerra i doble)*/
    case 0: 
    case 1:
    case 6:
      girar();
      break;
    case 2: 	
	fletxaRecta();
      break;
    case 7: 
	fletxaTriple();
      break;
    default:
      break;
    }
}

void Scene::fletxaRecta()
{
  if(velocitat!=0 && direccio==2) 
    {
      veh.setVelocitat(velocitat);
      velocitat=0;
    }

  //2 fletxes rectes perpendiculars
  if(direccio!=2) girar();
  else if (circuit[tram].seguentXPOS()) caminaXPOS();
  else if (circuit[tram].seguentXNEG()) caminaXNEG();
  else if (circuit[tram].seguentZPOS()) caminaZPOS();
  else if (circuit[tram].seguentZNEG()) caminaZNEG();
  
}

void Scene::fletxaTriple()
{
 if (direccio!=2) 
  girar();
  else
  {
    if (circuit[tram].seguentXPOS()) caminaXPOS();
    else if (circuit[tram].seguentXNEG()) caminaXNEG();
    else if (circuit[tram].seguentZPOS()) caminaZPOS();
    else if (circuit[tram].seguentZNEG()) caminaZNEG();
  }
}

void Scene::caminaXPOS()
{
  Point pos_veh = veh.getPos();
  pos_veh.x += veh.getVelocitat();
  veh.setPos(pos_veh);

  Point pos_seguent = circuit[circuit[tram].getSeguent(0)].getPosition();
  
  if (pos_veh.x >= pos_seguent.x - 0.5 - veh.getVelocitat())
    { 
      //Hem canviat de tram!
      orientacio_anterior = circuit[tram].getOrientation();
      tipus_anterior = circuit[tram].getObjectId();

      //Actualitzem l'estat del tram en que estem ara
      tram = circuit[tram].getSeguent(0);
      
      switch(circuit[tram].getObjectId())
	{
	case 2:
	  if(tipus_anterior==2)
		{
	    	switch((int)circuit[tram].getOrientation() - orientacio_anterior)
	      {
	      case -90: //De XPOS a ZPOS
		direccio = 0;
		break;
	      case 90: 
		//De XPOS a ZNEG
		direccio = 1; //Girem esquerra
		break;
	      case 270: 
		//De XPOS a ZPOS
		direccio = 0; // Girem dreta
		break;
	      default: direccio = 2; 
		break;
	      }
	  }
	  else{
	    direccio = 2;
	  }
	  break;
	case 6:
	  direccio = rand()%2;
	  break;
	case 7:
	  direccio = rand()%3;
	  break;
	default: direccio = 2; break;
	}
    }
}

void Scene::caminaXNEG()
{
  Point pos_veh = veh.getPos();
  pos_veh.x -= veh.getVelocitat();
  veh.setPos(pos_veh);
  
  Point pos_seguent = circuit[circuit[tram].getSeguent(1)].getPosition();
  
  if (pos_veh.x <= pos_seguent.x + 0.5 + veh.getVelocitat())
    { 
      //Hem canviat de tram!
      orientacio_anterior = circuit[tram].getOrientation();
      tipus_anterior = circuit[tram].getObjectId();

      //Actualitzem l'estat del tram en que estem ara
      tram = circuit[tram].getSeguent(1);
 
      switch(circuit[tram].getObjectId())
	{
	case 2:
	  if(tipus_anterior==2){
	    switch((int)circuit[tram].getOrientation() - orientacio_anterior)
	      {
	      case -90: 
		//De XNEG a ZNEG
		direccio = 0; //Girem dreta
		break;
	      case 90: //De XNEG a ZPOS
		direccio = 1; //Girem esquerra
		break;
	      default: direccio = 2; break;
	      }
	  }
	  else{
	    direccio = 2;
	  }
	    break;
	case 6:
	  direccio = rand()%2;
	  break;
	case 7:
	  direccio = rand()%3;
	  break;
	default: direccio = 2; break;
	}
    }
}

void Scene::caminaZPOS()
{ 
  Point pos_veh = veh.getPos();
  pos_veh.z += veh.getVelocitat();
  veh.setPos(pos_veh);
  
  Point pos_seguent = circuit[circuit[tram].getSeguent(2)].getPosition();
  
  if (pos_veh.z >= pos_seguent.z - 0.5 - veh.getVelocitat())
    { 
      //Hem canviat de tram!
      orientacio_anterior = circuit[tram].getOrientation();
      tipus_anterior = circuit[tram].getObjectId();

      //Actualitzem l'estat del tram en que estem ara
      tram = circuit[tram].getSeguent(2); 

      switch(circuit[tram].getObjectId())
	{
	case 2:
	  if(tipus_anterior==2){
	    switch((int)circuit[tram].getOrientation() - orientacio_anterior)
	      {
	      case -270: 
		//De ZPOS a XPOS
		direccio = 1; //Girem esquerra
		break;
	      case -90: //De ZPOS a ZNEG
		direccio = 0; //Girem dreta
		break;
	      default: direccio = 2; break;
	      }
	  }
	  else{
	    direccio = 2;
	  }
	  break;
	case 6:
	  direccio = rand()%2;
	  break;
	case 7:
	  direccio = rand()%3;
	  break;
	default: direccio = 2; break;
	}
    }
}

void Scene::caminaZNEG()
{
  Point pos_veh = veh.getPos();
  pos_veh.z -= veh.getVelocitat();
  veh.setPos(pos_veh);

  Point pos_seguent = circuit[circuit[tram].getSeguent(3)].getPosition();
  
  if (pos_veh.z <= pos_seguent.z + 0.5 + veh.getVelocitat())
    {
      //Hem canviat de tram!
      orientacio_anterior = circuit[tram].getOrientation();
      tipus_anterior = circuit[tram].getObjectId();

      //Actualitzem l'estat del tram en que estem ara
      tram = circuit[tram].getSeguent(3);

      switch(circuit[tram].getObjectId())
	{
	case 2:
	  if(tipus_anterior==2){
	    switch((int)circuit[tram].getOrientation() - orientacio_anterior)
	      {
	      case -90: //De ZNEG a XPOS
		direccio = 0; //Girem dreta
		break;
	      case 90: //De ZNEG a XNEG
		direccio = 1; //Girem esquerra
		break;
	      default: direccio = 2; break;
	      }
	  }
	  else{
	    direccio = 2;
	  }
	  break;
	case 6:
	  direccio = rand()%2;
	  break;
	case 7:
	  direccio = rand()%3;
	  break;
	default: direccio = 2; break;
	}
    }
  
}


void Scene::girar()
{
  Point pos_veh = veh.getPos();
  int ori = (int)circuit[tram].getOrientation();

  //Estem en un tram Perpendicular
  if(direccio!=2) 
    ori = orientacio_anterior; 
  
  switch(ori)
    {
    case 0: 
    //Canvi a XNEG 
      if(circuit[tram].getObjectId()==ESQUERRA || direccio ==1)
      	{ 
	  tram_ori += veh.getVCurva();

	  Point centreGir;
	  centreGir.x=circuit[tram].getPosition().x+(circuit[tram].getSize().x/2);
	  centreGir.z=circuit[tram].getPosition().z+(circuit[tram].getSize().z/2);
	  
	  pos_veh.x = centreGir.x+(sin((veh.getOrientation()-dif_ori)*DEG2RAD))*0.5;
	  pos_veh.z = centreGir.z+(cos((veh.getOrientation()-dif_ori)*DEG2RAD))*0.5;
	  veh.setPos(pos_veh);

	  int graus = veh.getOrientation()+veh.getVCurva();
	  veh.setOrientation(graus%360);

	  ori_veh += veh.getVCurva();
	  
	  if(ori_veh==90)
	    {
	      tipus_anterior = circuit[tram].getObjectId();
      	      tram = circuit[tram].getSeguent(2); //ZPOS
	      direccio = 2;
	      ori_veh=0;
	    }
	  else if(ori_veh+veh.getVCurva()>90)
	    {
	      pos_veh.x=circuit[tram].getPosition().x;
	      pos_veh.z=circuit[tram].getPosition().z+(circuit[tram].getSize().x/2);
	      veh.setPos(pos_veh);
	      veh.setOrientation(90);
	      
	      tipus_anterior = circuit[tram].getObjectId();
      	      tram = circuit[tram].getSeguent(2); //ZPOS
	      direccio = 2;
	      ori_veh=0;
	    }
      	}
      
      if(direccio ==0)
      	{ 
	//Canvi a ZNEG
	  tram_ori -= veh.getVCurva();

	  Point centreGir;
	  centreGir.x=circuit[tram].getPosition().x+(circuit[tram].getSize().x/2);
	  centreGir.z=circuit[tram].getPosition().z-(circuit[tram].getSize().z/2);
	  
	  pos_veh.x = centreGir.x-(sin((veh.getOrientation()-dif_ori)*DEG2RAD))*0.5;
	  pos_veh.z = centreGir.z-(cos((veh.getOrientation()-dif_ori)*DEG2RAD))*0.5;
	  veh.setPos(pos_veh);

	  int graus = veh.getOrientation()-veh.getVCurva();
	  veh.setOrientation(graus%360);

	  ori_veh += veh.getVCurva();
	 
	  if(ori_veh==90)
	    {
	      tipus_anterior = circuit[tram].getObjectId();
      	      tram = circuit[tram].getSeguent(3); //ZNEG
	      direccio = 2;
	      ori_veh=0;
	    }
	  else if(ori_veh+veh.getVCurva()>90)
	    {
	      pos_veh.x=circuit[tram].getPosition().x;
	      pos_veh.z=circuit[tram].getPosition().z-(circuit[tram].getSize().x/2);
	      veh.setPos(pos_veh);
	      veh.setOrientation(270);
	      
	    tipus_anterior = circuit[tram].getObjectId();
      	      tram = circuit[tram].getSeguent(3); //ZNEG
	      direccio = 2;
	      ori_veh=0;
	    }
	  
      	}

      break;
      
    case 90:
      if(direccio == 0)
	{ 
	//Canvi a XNEG
	  tram_ori -= veh.getVCurva();
	  
	  Point centreGir;
	  centreGir.x=circuit[tram].getPosition().x-(circuit[tram].getSize().x/2);
	  centreGir.z=circuit[tram].getPosition().z-(circuit[tram].getSize().z/2);
	  
	  pos_veh.x = centreGir.x-(sin((veh.getOrientation()-dif_ori)*DEG2RAD))*0.5;
	  pos_veh.z = centreGir.z-(cos((veh.getOrientation()-dif_ori)*DEG2RAD))*0.5;
	  veh.setPos(pos_veh);
	  
	  int graus = veh.getOrientation() - veh.getVCurva();
	  veh.setOrientation(graus%360);
	  
	  ori_veh += veh.getVCurva();
	  
	  if(ori_veh==90)
	    {
	      tipus_anterior = circuit[tram].getObjectId();
	      tram = circuit[tram].getSeguent(0); //XPOS
	      direccio = 2;
	      ori_veh=0;
	    }
	  else if(ori_veh+veh.getVCurva()>90)
	    {
	      pos_veh.x=circuit[tram].getPosition().x-(circuit[tram].getSize().x/2);
	      pos_veh.z=circuit[tram].getPosition().z;
	      veh.setPos(pos_veh);
	      veh.setOrientation(0);
	      
	      tipus_anterior = circuit[tram].getObjectId();
	      tram = circuit[tram].getSeguent(0); //XPOS
	      direccio = 2;
	      ori_veh=0;
	    }
	}
      else if(direccio == 1 || circuit[tram].getObjectId()==ESQUERRA) //Gir esquerra
	{ 
	//Canvi a XPOS	  
	  tram_ori += veh.getVCurva();

	  Point centreGir;
	  centreGir.x=circuit[tram].getPosition().x+(circuit[tram].getSize().x/2);
	  centreGir.z=circuit[tram].getPosition().z-(circuit[tram].getSize().z/2);
	  
	  pos_veh.x = centreGir.x+(sin((veh.getOrientation()-dif_ori)*DEG2RAD))*0.5;
	  pos_veh.z = centreGir.z+(cos((veh.getOrientation()-dif_ori)*DEG2RAD))*0.5;
	  veh.setPos(pos_veh);
	  
	  int graus = veh.getOrientation()+veh.getVCurva();
	  veh.setOrientation(graus%360);
	  
	  
	  ori_veh += veh.getVCurva();
	  
	  if(circuit[tram].getObjectId()==6 && ori_veh== 90) 
	    {
	      tipus_anterior = circuit[tram].getObjectId();
	      tram = circuit[tram].getSeguent(1); 	  
	      direccio = 2;
	      ori_veh=0;
	    }
	  else if(circuit[tram].getObjectId()==6 && ori_veh+veh.getVCurva()>90)
	    {
	      pos_veh.x=circuit[tram].getPosition().x;
	      pos_veh.z=circuit[tram].getPosition().z-(circuit[tram].getSize().x/2);
	      veh.setPos(pos_veh);
	      veh.setOrientation(180);
	      
	      tipus_anterior = circuit[tram].getObjectId();
	      tram = circuit[tram].getSeguent(1); 	  
	      direccio = 2;
	      ori_veh=0;
	      
	    }
	  else if(ori_veh==90)
	    {
	      tipus_anterior = circuit[tram].getObjectId();
	      tram = circuit[tram].getSeguent(0); 	  
	      direccio = 2;
	      ori_veh=0;
	    }
	  else if(ori_veh+veh.getVCurva()>90)
	    {
	      pos_veh.x=circuit[tram].getPosition().x;
	      pos_veh.z=circuit[tram].getPosition().z-(circuit[tram].getSize().x/2);
	      veh.setPos(pos_veh);
	      veh.setOrientation(180);
	      
	      tipus_anterior = circuit[tram].getObjectId();
	      tram = circuit[tram].getSeguent(0); 	  
	      direccio = 2;
	      ori_veh=0;
	      
	    }
	}
      break;
      
    case 180:
      
      if(circuit[tram].getObjectId()==DRETA || direccio ==0)
      	{ 	  
	  tram_ori -= veh.getVCurva();

	  Point centreGir;
	  centreGir.x=circuit[tram].getPosition().x-(circuit[tram].getSize().x/2);
	  centreGir.z=circuit[tram].getPosition().z+(circuit[tram].getSize().z/2);
	  
	  pos_veh.x = centreGir.x-(sin((veh.getOrientation()-dif_ori)*DEG2RAD))*0.5;
	  pos_veh.z = centreGir.z-(cos((veh.getOrientation()-dif_ori)*DEG2RAD))*0.5;
	  veh.setPos(pos_veh);

	  if(veh.getOrientation()==0){ veh.setOrientation(360-veh.getVCurva()); }
	  else{ veh.setOrientation(veh.getOrientation()-veh.getVCurva());}

	  ori_veh += veh.getVCurva();

	  if(ori_veh>=90)
	    {
	      tipus_anterior = circuit[tram].getObjectId();
	      tram = circuit[tram].getSeguent(2); //ZPOS
	      direccio = 2;
	      ori_veh=0;
	    }
	  else if(ori_veh+veh.getVCurva()>90)
	    {
	      pos_veh.x=circuit[tram].getPosition().x;
	      pos_veh.z=circuit[tram].getPosition().z+(circuit[tram].getSize().x/2);
	      veh.setPos(pos_veh);
	      veh.setOrientation(90);

	      tipus_anterior = circuit[tram].getObjectId();
	      tram = circuit[tram].getSeguent(2); //ZPOS
	      direccio = 2;
	      ori_veh=0;
	    
	    }
      	}
      if(circuit[tram].getOrientation()==270 || circuit[tram].getObjectId()==ESQUERRA)
      	{ //Canvi a ZNEG

	  tram_ori += veh.getVCurva();

	  Point centreGir;
	  centreGir.x=circuit[tram].getPosition().x-(circuit[tram].getSize().x/2);
	  centreGir.z=circuit[tram].getPosition().z-(circuit[tram].getSize().z/2);
	  
	  pos_veh.x = centreGir.x+(sin((veh.getOrientation()-dif_ori)*DEG2RAD))*0.5;
	  pos_veh.z = centreGir.z+(cos((veh.getOrientation()-dif_ori)*DEG2RAD))*0.5;
	  veh.setPos(pos_veh);
	  
	 int graus = veh.getOrientation()+veh.getVCurva();
	 veh.setOrientation(graus%360);
	  
	  ori_veh += veh.getVCurva();

	  if(ori_veh==90)
	    {
	      tipus_anterior = circuit[tram].getObjectId();
	      tram = circuit[tram].getSeguent(3); 
	      direccio = 2;
	      ori_veh=0; 
	    }
	  else if(ori_veh+veh.getVCurva()>90)
	    {
	      pos_veh.x=circuit[tram].getPosition().x;
	      pos_veh.z=circuit[tram].getPosition().z-(circuit[tram].getSize().x/2);
	      veh.setPos(pos_veh);
	      veh.setOrientation(270);
	      
	      tipus_anterior = circuit[tram].getObjectId();
	      tram = circuit[tram].getSeguent(3); 
	      direccio = 2;
	      ori_veh=0; 
	    
	    }
      	}
      
      break;
      
    case 270:
   
      if(circuit[tram].getObjectId()==ESQUERRA)
	{ 
	//Canvi ZNEG a XPOS
		  
	  tram_ori += veh.getVCurva();

	  Point centreGir;
	  centreGir.x=circuit[tram].getPosition().x-(circuit[tram].getSize().x/2);
	  centreGir.z=circuit[tram].getPosition().z+(circuit[tram].getSize().z/2);
	  
	  pos_veh.x = centreGir.x+(sin((veh.getOrientation()-dif_ori)*DEG2RAD))*0.5;
	  pos_veh.z = centreGir.z+(cos((veh.getOrientation()-dif_ori)*DEG2RAD))*0.5;
	  veh.setPos(pos_veh);
	  
	  int graus = veh.getOrientation()+veh.getVCurva();
	  veh.setOrientation(graus%360);
	  
	  ori_veh += veh.getVCurva();

	  if(ori_veh==90)
	    {
	      tipus_anterior = circuit[tram].getObjectId();
	      tram = circuit[tram].getSeguent(1); //XNEG
	      direccio = 2;
	      ori_veh=0;
	    }
	  else if(ori_veh+veh.getVCurva()>90)
	    {
	      pos_veh.x=circuit[tram].getPosition().x-(circuit[tram].getSize().x/2);
	      pos_veh.z=circuit[tram].getPosition().z;
	      veh.setPos(pos_veh);
	      veh.setOrientation(0);
	      
	      tipus_anterior = circuit[tram].getObjectId();
	      tram = circuit[tram].getSeguent(1); //XNEG
	      direccio = 2;
	      ori_veh=0;
	    
	    }
	}

      if(circuit[tram].getObjectId()==DRETA)
	{ 
	//Canvi de XNEG a ZNEG
	  tram_ori -= veh.getVCurva();

	  Point centreGir;
	  centreGir.x=circuit[tram].getPosition().x+(circuit[tram].getSize().x/2);
	  centreGir.z=circuit[tram].getPosition().z+(circuit[tram].getSize().z/2);
	  
	  pos_veh.x = centreGir.x-(sin((veh.getOrientation()-dif_ori)*DEG2RAD))*0.5;
	  pos_veh.z = centreGir.z-(cos((veh.getOrientation()-dif_ori)*DEG2RAD))*0.5;
	  veh.setPos(pos_veh);
	  
	  int graus = veh.getOrientation()-veh.getVCurva();
	  veh.setOrientation(graus%360);
	  
	  ori_veh += veh.getVCurva();
	  
	  if(ori_veh==90)
	    {
	      tipus_anterior = circuit[tram].getObjectId();
	      tram = circuit[tram].getSeguent(0); //XNEG
	      direccio = 2;
	      ori_veh=0;
	    }
	  else if(ori_veh+veh.getVCurva()>90)
	    {
	      pos_veh.x=circuit[tram].getPosition().x+(circuit[tram].getSize().x/2);
	      pos_veh.z=circuit[tram].getPosition().z;
	      veh.setPos(pos_veh);
	      veh.setOrientation(180);
	      
	      tipus_anterior = circuit[tram].getObjectId();
	      tram = circuit[tram].getSeguent(0); //XNEG
	      direccio = 2;
	      ori_veh=0;
	    }
	}
 
      break;
      
    default: break;
    }  
}

bool Scene::getRunning()
{
  return engegat;
}

bool Scene::getVehicle()
{
  return vehicle;
}


void Scene::setRunning(bool aturat)
{
  engegat = aturat;
}

void Scene::reiniciarV()
{
  //Movem el vehicle a la posició del tram 0, i el pugem a nivell del terra
  //El nivell és 0.1, fer: cout << circuit[0].getSize() << endl;
  //També l'orientem correctament

  tram=0;
  Point pos = circuit[tram].getPosition();
  pos.y += 0.1;
  veh.setPos(pos);
  veh.setOrientation(orientacio_inicial);
  ori_veh = 0.0; 
  dif_ori=0;
  orientacio_anterior = 180;
  direccio = 2;
  Render();
}

void Scene::orientaVehicle(int ori)
{
  veh.setOrientation(veh.getOrientation() + ori - dif_ori);
  dif_ori = ori;
}

void Scene::setVelocitat(int v)
{
  velocitat = v;
}

void Scene::setVisible(bool visible)
{
  vehicle = visible;
}

Point Scene::getVehPos()
{
  return veh.getPos();
}

int Scene::getOrientacio()
{
  return (veh.getOrientation() + dif_ori) % 360;
}

Point Scene::getVehOrientation()
{
  return veh.pos + Point(
	  - cos((double)tram_ori*M_PI/180.)*5,
	  0.5,
	  + sin((double)tram_ori*M_PI/180.)*5
	  );
}

void Scene::suavitzar(bool suav)
{
  suavizat = suav;
}

float Scene:: midaTram()
{
  //Retorna la mida de l'eix X d'un tram
  return circuit[tram].getSize().x;
}

Point Scene:: orientLlum()
{
  //Retorna la direccio en que ha d'anar la llum
  return Point(-cos((double)tram_ori *M_PI/180.),
	       -.5,
	       sin((double)tram_ori*M_PI/180.)
	       );
}

void Scene::RenderSelect()
{
  //Guarda els "noms" dels objectes que poden ser copiats
  vector <Referencia> ::iterator iter;
  int i = 0;
  for (iter=lreferencies.begin(); iter<lreferencies.end(); iter++)
    {
      glPushName(i);
      (*iter).Render(lobjects,suavizat);
      glPopName();
      i++;
    }
}

void Scene:: addObjIni(int name)
{
  //Fem la còpia per primera vegada
  Referencia copia = lreferencies[name];
  lreferencies.push_back(copia);	
}

void Scene:: setPosObjXZ(Point p)
{
  //Modifiquem la posicio en x-z
  Point pos = lreferencies[lreferencies.size()-1].getPosition();
  pos.x = p.x;
  pos.z = p.z;
  lreferencies[lreferencies.size()-1].setPosition(pos);
}

void Scene::deixaObj()
{
  //Posicio final de la copia
  Point pos = lreferencies[lreferencies.size()-1].getPosition();
  pos.y--;
  lreferencies[lreferencies.size()-1].setPosition(pos);
}

void Scene::selectCancel()
{
  //Eliminar la copia del vector lreferencies
  lreferencies.pop_back();
}

Point Scene:: getPosObj()
{
  //Retorna la poiscio de la copia
  Referencia copia = lreferencies[lreferencies.size()-1];
  return copia.getPosition();
}

void Scene:: pintarObj()
{
  //Fa el pintat de la copia en l'escena
  lreferencies[lreferencies.size()-1].Render(lobjects,suavizat);
}
	
vector <Referencia> Scene::getReferencies()
{
  //Retorna el vector lreferencies
  return lreferencies;
}
