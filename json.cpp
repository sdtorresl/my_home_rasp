#include "lib/json/document.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>

using namespace std;
using namespace rapidjson;

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

/** Main function
 */
int main() {
  string json = httpRequest("?function=getState\\&room_id=1\\&home_id=1\\home_password=myhome_pass");

  cout<<json;
	Document d;
  d.Parse<0>(json.c_str());

  cout<< d["room"]["name"].GetString()<<endl;
  cout<< d["success"].GetString()<<endl;
  cout<< d["message"].GetString()<<endl;

  return 0;
}