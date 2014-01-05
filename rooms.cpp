#include <iostream> 
#include <string>

using namespace std;

const int constant = 3;

unsigned int idHC = 1;
string homeNameC = "Casa David";
bool modeC = false;

unsigned int node = 1;

unsigned int idC = 1;
string nameC = "Comedor";
int levelC = 50;
bool controlC = true;
 
class room {
   private:
      // Datos miembro de la clase "room"
      unsigned int id;  // id de la casa
      string name;      // nombre de la habitación
      int level;        // nivel de iluminación de la habitación
      bool control;     // control de la habitación: control por sistema o manual (on-off)
      
   public:
      // Constructor sin parámetros
      room(){}
      // Constructor parametrizado
      room(unsigned int idRoom, string nameRead, int levelRead, bool controlRead) : id(idRoom), name(nameRead), level(levelRead), control(controlRead) {}
      // Destructor
      ~room(){}
      /* Funciones miembro de la clase "room" */
      // Funciones para obtener los valores almacenados en el room
      unsigned int getId(){
         return id;
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
      // Funciones para almacenar los valores del room
      void setId(unsigned int idRead);
      void setName(string nameRead);
      void setLevel(int levelRead);
      void setControl(bool controlRead);
      // Función para almacenar la información recibida en las variables
      void setData(unsigned int idRead, string nameRead, int levelRead, bool controlRead);
};

void room::setId(unsigned int idRead){
   id = idRead;
}
void room::setName(string nameRead){
   name = nameRead;
}
void room::setLevel(int levelRead){
   level = levelRead;
}
void room::setControl(bool controlRead){
   control = controlRead;
}
void room::setData(unsigned int idRead, string nameRead, int levelRead, bool controlRead){
   id = idRead;
   name = nameRead;
   level = levelRead;
   control = controlRead;
}

class home {
   private:
      // Datos miembro de la clase "home"
      unsigned int homeId;
      string homeName;
      bool mode;
      room* nodes;
   public:
      // Constructror parametrizado
      home(unsigned int homeIdRead, string homeNameRead, bool modeRead) : homeId(homeIdRead), homeName(homeNameRead), mode(modeRead) {}
      // Destructor
      ~home() {}
      /* Funciones miembro de la clase "home" */
      // Funciones para obtener los valores de home
      unsigned int getHomeId(){
         return homeId;
      }
      string getHomeName(){
         return homeName;
      }
      bool getMode(){
         return mode;
      }
      // Funciones para definir valores de home
      void setHomeId(unsigned int homeIdRead);
      void setHomeName(string homeNameRead);
      void setMode(bool modeRead);
      // Función para llenar información del nodo
      void setNodes(unsigned int idRoom, string nameRead, int levelRead, bool controlRead);
      // Función para crear el número de nodos necesarios
      void createNodes(int roomsNumber);
      // Función para imprimir la información almacenada de la casa
      void printHomeData();
      // Función para imprimir la información almacenada del nodo
      void printNodeData(int nodeNumber);
};

void home::setHomeId(unsigned int homeIdRead){
   homeId = homeIdRead;
}
void home::setHomeName(string homeNameRead){
   homeName = homeNameRead;
}
void home::setMode(bool modeRead){
   mode = modeRead;
}
void home::setNodes(unsigned int idRoom, string nameRead, int levelRead, bool controlRead){
   nodes[idRoom].setData(idRoom,nameRead,levelRead,controlRead);
}
void home::createNodes(int roomsNumber){
   nodes = new room[roomsNumber];
}

void home::printHomeData(){
   cout<<"House ID: "<<homeId<<endl;
   cout<<"House Name: "<<homeName<<endl;
   cout<<"Control Mode: "<<mode<<endl;
}

void home::printNodeData(int nodeNumber){

   cout<<"Node ID: "<< nodes[nodeNumber].getId();
   cout<<endl;
   cout<<"Node Name: "<< nodes[nodeNumber].getName();
   cout<<endl;
   cout<<"Node Level: "<< nodes[nodeNumber].getLevel();
   cout<<endl;
   cout<<"Node Control: "<< nodes[nodeNumber].getControl();
   cout<<endl;
}

int main() {
   home house(idHC,homeNameC,modeC);
   house.printHomeData();

   house.createNodes(constant);
   house.setNodes(node,nameC,levelC,controlC);

   house.printNodeData(node);
   return 0;
}