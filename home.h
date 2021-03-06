#include <iostream> 
#include <string>

using namespace std;

class Room {
   private:
      // Datos miembro de la clase "Room"
      unsigned int id;  // id de la habitación
      unsigned int DL;  // DL del Xbee
      string name;      // nombre de la habitación
      int level;        // nivel de iluminación de la habitación
      bool control;     // control de la habitación: control por sistema o manual (on-off)
      bool automatic;   // control de la habitación desde el celular: por teléfono o por sensores
      
   public:
      // Constructor sin parámetros
      Room(){}
      // Constructor parametrizado
      Room(unsigned int idRoom, unsigned int DLRead, string nameRead, int levelRead, bool controlRead, bool automaticRead) : id(idRoom), DL(DLRead), name(nameRead), level(levelRead), control(controlRead), automatic(automaticRead) {}
      // Destructor
      ~Room(){}
      /* Funciones miembro de la clase "Room" */
      // Funciones para obtener los valores almacenados en el Room
      unsigned int getId(){
         return id;
      }
      unsigned int getDL(){
         return DL;
      }
      string getName(){
         return name;
      }
      int getLevel(){
         return level;
      }
      bool getControl(){
         return control;
      }
      bool getAutomatic(){
         return automatic;
      }
      // Funciones para almacenar los valores del Room
      void setId(unsigned int idRead);
      void setDL(unsigned int DLRead);
      void setName(string nameRead);
      void setLevel(int levelRead);
      void setControl(bool controlRead);
      void setAutomatic(bool automaticRead);
      // Función para almacenar la información recibida en las variables
      void setData(unsigned int idRead, unsigned int DLRead, string nameRead, int levelRead, bool controlRead, bool automaticRead);
};

void Room::setId(unsigned int idRead){
   id = idRead;
}

void Room::setDL(unsigned int DLRead){
   DL = DLRead;
}

void Room::setName(string nameRead){
   name = nameRead;
}

void Room::setLevel(int levelRead){
   level = levelRead;
}

void Room::setControl(bool controlRead){
   control = controlRead;
}

void Room::setAutomatic(bool automaticRead){
   automatic = automaticRead;
}

void Room::setData(unsigned int idRead, unsigned int DLRead, string nameRead, int levelRead, bool controlRead, bool automaticRead){
   id = idRead;
   DL = DLRead;
   name = nameRead;
   level = levelRead;
   control = controlRead;
   automatic = automaticRead;
}

class Home {
   private:
      // Datos miembro de la clase "Home"
      unsigned int homeId;
      string homeName;
      bool mode;
      Room* nodes;
   public:
      // Constructor sin parámetros
      Home(){}
      // Constructor parametrizado
      Home(unsigned int homeIdRead, string homeNameRead, bool modeRead) : homeId(homeIdRead), homeName(homeNameRead), mode(modeRead) {}
      // Destructor
      ~Home() {}
      /* Funciones miembro de la clase "Home" */
      // Funciones para obtener los valores de Home
      unsigned int getHomeId(){
         return homeId;
      }
      string getHomeName(){
         return homeName;
      }
      bool getMode(){
         return mode;
      }
      // Funciones para definir valores de Home
      void setHomeId(unsigned int homeIdRead);
      void setHomeName(string homeNameRead);
      void setMode(bool modeRead);
      // Funciones que devuelven los valores de los nodos
      unsigned int getNodeId(int nodeNumber){
         return nodes[nodeNumber].getId();
      }
      unsigned int getNodeDL(int nodeNumber){
         return nodes[nodeNumber].getDL();
      }
      string getNodeName(int nodeNumber){
         return nodes[nodeNumber].getName();
      }
      int getNodeLevel(int nodeNumber){
         return nodes[nodeNumber].getLevel();
      }
      bool getNodeControl(int nodeNumber){
         return nodes[nodeNumber].getControl();
      }
      bool getNodeAutomatic(int nodeNumber){
         return nodes[nodeNumber].getAutomatic();
      }

      // Función para llenar información del nodo
      void setNodes(int nodeNumber, unsigned int idRead, unsigned int DLRead, string nameRead, int levelRead, bool controlRead, bool automaticRead);
      // Función para crear el número de nodos necesarios
      void createNodes(int roomsNumber);
      // Función para imprimir la información almacenada de la casa
      void printHomeData();
      // Función para imprimir la información almacenada del nodo
      void printNodeData(int nodeNumber);
};

void Home::setHomeId(unsigned int homeIdRead){
   homeId = homeIdRead;
}

void Home::setHomeName(string homeNameRead){
   homeName = homeNameRead;
}

void Home::setMode(bool modeRead){
   mode = modeRead;
}

void Home::setNodes(int nodeNumber, unsigned int idRead, unsigned int DLRead, string nameRead, int levelRead, bool controlRead, bool automaticRead){
   nodes[nodeNumber].setData(idRead,DLRead,nameRead,levelRead,controlRead,automaticRead);
}

void Home::createNodes(int roomsNumber){
   nodes = new Room[roomsNumber];
}

void Home::printHomeData(){
   cout<<"House ID: "<<homeId<<endl;
   cout<<"House Name: "<<homeName<<endl;
   // cout<<"Control Mode: "<<mode<<endl;
}

void Home::printNodeData(int nodeNumber){
   cout<<"Node ID: "<< nodes[nodeNumber].getId();
   cout<<endl;
   cout<<"Node DL: "<< nodes[nodeNumber].getDL();
   cout<<endl;
   cout<<"Node Name: "<< nodes[nodeNumber].getName();
   cout<<endl;
   cout<<"Node Level: "<< nodes[nodeNumber].getLevel();
   cout<<endl;
   cout<<"Node Control: "<< nodes[nodeNumber].getControl();
   cout<<endl;
   cout<<"Node Automatic: "<< nodes[nodeNumber].getAutomatic();
   cout<<endl;
}