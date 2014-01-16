#include "lib/json/document.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>

using namespace std;
using namespace rapidjson;

const unsigned int HOME_ID = 1;
string URL = "http://myhome.exeamedia.com";
string FILE_NAME = "json.txt";

string httpRequest(string parameters);
string readFile();
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


