#ifndef _Scene_H_
#define _Scene_H_
#include "referencia.h"
#include "object.h"
#include "vehicle.h"
#include "tram.h"
#include <QtOpenGL/qgl.h>


class Scene
{
  friend class XML;
  friend class Tram;
 private:

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
  std::vector<Referencia> lreferencies;
  std::vector<Tram> circuit;

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
  void carregaVehicle (const char* filename);
  void orientaVehicle (int graus);
};

#endif

