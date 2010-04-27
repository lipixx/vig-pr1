#ifndef _Scene_H_
#define _Scene_H_
#include "referencia.h"
#include "object.h"
#include "vehicle.h"
#include "tram.h"
#include <QtOpenGL/qgl.h>
#include <cmath>

#define PI 3.1415926535898
#define DEG2RAD PI/180 
#define RAD2DEG 180/PI

class Scene
{
  friend class XML;
  friend class Tram;
 private:

  bool vehicle, engegat, suavizat;
  int tram, orientacio_anterior, direccio, tipus_anterior, orientacio_inicial, dif_ori, velocitat;

  /*
    direccio = 0 => gir dreta
    direccio = 1 => gir esquerra
    direccio = 2 => segueix recta
  */
  
  void moureVehicle();
  
  void fletxaRecta();
  void fletxaDreta();
  void fletxaEsquerra();
  void fletxaDoble();
  void fletxaTriple();
  void fletxaRecte();

  void caminaXPOS();
  void caminaXNEG();
  void caminaZPOS();
  void caminaZNEG();
  
  void girar();
  
  // Aix� �s per a millorar la cerca dels objectes dins del vector.
  static int DRETA;
  static int ESQUERRA;
  static int RECTA;
  static int ARBRE;
  static int PEDRES;
  static int CASA;
  static int DOBLE;
  static int TRIPLE;
  static int FANAL;

	
  // Tindrem un vector amb els models geom�trics dels objectes de l'escena
  // i els altres dos amb inst�ncies seves (inst�ncies o refer�ncies a 
  // objectes).
  // El vector circuit cont� els trams de la carretera i el vector 
  // lreferencies tota la resta d'inst�ncies d'objectes de l'escena 
  // (arbres, cases, fanals, etc...)

  std::vector<Object> lobjects;
  /* lobjects: �s un vector que cont� el model geom�tric dels objectes b�sics 
     de qu� consta l'escena (els trams de carretera, les cases, els arbres, etc.).*/
  std::vector<Referencia> lreferencies;
  /* lreferencies: �s un vector que cont� inst�ncies dels objectes b�sics i representa
     el conjunt d'objectes de l'escena que no s�n trams de carretera.*/
  std::vector<Tram> circuit;
  /* circuit: �s un vector que cont� inst�ncies dels trams de carretera.*/

  Vehicle veh;

 public:
  static MaterialLib matlib;	  // col�lecci� de materials 
  static void transformPoint(Point &p);

  Scene();

  void Init();
  void Render();
  void AddObject(Object &);
  void AddObject(Referencia &);
  void AddObject(Tram &);

  void CalculaEsfera (double &radi, Point &centreEscena);

  void carregarVehicle(const char * file);
  
  bool getRunning();
  bool getVehicle();
  void setRunning(bool aturat);
  void setVisible(bool visible);
  void reiniciarV();

  void orientaVehicle(int ori);
  float ori_veh, tram_ori;

  void setVelocitat(int v);

  Point getVehPos();
  int getOrientacio();
  Point getVehOrientation();
  
  
  void suavitzar(bool suav);

  float midaTram();


 //orientLlum - ens retorna la direccio en que ha d'anar la llum
  Point orientLlum();

  //RenderSelect - guarda els "noms" dels objectes que poden ser copiats
  void RenderSelect();

  //addObjIni - fem la c�pia per primera vegada
  void addObjIni(int name);

  //setPosObjXZ - modifiquem la posicio en x-z
  void setPosObjXZ(Point p);

  //deixaObj - posicio final de la copia NOM�S USAT EN SESSI�3
  void deixaObj();

  //selectCancel - eliminar la copia del vector lreferencies
  void selectCancel();

  //getPosObj - retorna la poiscio de la copia
  Point getPosObj();
 //pintarObj - realiza el pintat de la copia en l'escena (amb filferros)
  void pintarObj();

 //getReferencies - retorna el vector lreferencies
  vector <Referencia> getReferencies();

 };

#endif

