#include "lib/json/document.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include "home.h"

using namespace std;
using namespace rapidjson;

const unsigned int HOME_ID = 1;
string URL = "http://myhome.exeamedia.com";
string FILE_NAME = "json.txt";

string readFile();
string httpRequest(string parameters);
char* stringToChar(string str);

string httpRequest(string parameters) {
	// Make URL for request
    string shellCmd = "curl -s " + URL + parameters + " > " + FILE_NAME;

	// Send command to shell
    char *cmd = stringToChar(shellCmd);
    system(cmd);

    string json = readFile();

	return json;
}

/** Read a file and return a string with 
 * 	the content
 */
string readFile() {
	string line;
	string content = "";

	char *file = stringToChar(FILE_NAME);
  	
  	ifstream jsonFile (file);
  	if (jsonFile.is_open()) {
    	while ( getline (jsonFile, line) ) {
			content += line + '\n';
    	}
    	jsonFile.close();
  	}
	else content = "Unable to open file"; 

	return content;
}

/** Convert string to char
 */
char* stringToChar(string str) {
	char c[1024];
	strncpy(c, str.c_str(), sizeof(c));
	c[sizeof(c) - 1] = 0;

	char *p = c;
	return p;
}

char* appendCharToCharArray(char* array, char a) {
  size_t len = strlen(array);

  char* ret = new char[len+2];

  strcpy(ret, array);    
  ret[len] = a;
  ret[len+1] = '\0';

  return ret;
}

/** Main function
 */
int main() {

  string json = httpRequest("?function=getHomeState\\&room_id=1\\&home_id=1\\&home_password=myhome_pass");

	Document d;
  d.Parse<0>(json.c_str());


  int homeID, roomID, roomState, nodes, roomRealID;
  bool success, control, mode;
  string message, homeName, roomName;

  success = d["success"].GetBool();
  message = d["message"].GetString();

  if (success) {
    // Get data from JSON document
    homeID = d["home"]["id"].GetInt();
    homeName = d["home"]["name"].GetString();
    mode = d["home"]["mode"].GetBool();
    nodes = d["home"]["nodes"].GetInt();

    Home house(homeID, homeName, mode);
    house.createNodes(nodes);
    house.printHomeData();

    char currentRoom[] = "room_";

    char cRoom[] = "room_1";
    for (int i = 0; i < nodes; ++i) {
      cRoom[5] = (char) (i + '0');

      cout<<cRoom<<endl;
      roomName = d["home"][cRoom]["name"].GetString();
      roomID = d["home"][cRoom]["id"].GetInt();
      control = d["home"][cRoom]["control"].GetBool();
      roomState = d["home"][cRoom]["state"].GetInt();

      house.setNodes(i, roomName, roomState, control);
      house.printNodeData(i);
    }
  }
  else {
    cout<<message<<endl;
  }
  
  //Remove json file and return
  system(stringToChar("rm " + FILE_NAME));
  return 0;
}