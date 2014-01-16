/*
 * my_home.cpp
 * 
 * Copyright 2013 Sergio Torres <sdtorresl@unal.edu.co>
 * 				  David Pinto 	<depintof@unal.edu.co>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include "lib/arduPi.h"
#include <iostream>
#include <string>
#include <algorithm>
#include "info.h"

#include "lib/json/document.h"
#include "home.h"


using namespace std;

string buffer;
string command;

int frameSize = 10;
char* frame = new char[frameSize];
// const int NODES = 5;

char addr[] = {0x50, 0x01};

char msg = 35;

// Size of the response from node
int validResponseSize = 7;
// Size of the node data
int nodeDataSize = 14;
// Node data
char nodeData[10] = {0,0,0,0,0,0,0,0,0,0};
// Buffer Node data
int buffNodeData[10] = {0,0,0,0,0,0,0,0,0,0};

// Stores the value of the json string
Document d;

/** Get hexadecimal number from char
 */
struct HexCharStruct {
  	unsigned char c;
  	HexCharStruct(unsigned char _c) : c(_c) { }
};

inline ostream& operator<<(ostream& o, const HexCharStruct& hs) {
  	return (o << hex << (int)hs.c);
}

inline HexCharStruct hex(unsigned char _c) {
  	return HexCharStruct(_c);
}

/** Compute checksum for send a frame to xbee 
 */
char checksum(char* frame) {
	char checksum = 0;
	for(int i=0; i<(int)*(frame+2); i++){
		checksum += *(frame+i+3);
	}
	checksum = 0xFF & checksum;
	checksum = 0xFF - checksum;
	return checksum;
}

/** Create a new frame for xbee with a address and a message
 */
void getFrame(char message, char addr[]){
	int totalSize = 10;
	char dh = addr[0];
	char dl = addr[1];

	frame[0] = 0x7E;
    frame[1] = 0x00;
    frame[2] = totalSize - 4;
	frame[3] = 0x01;
	frame[4] = 0x01;
	frame[5] = dh;
	frame[6] = dl;
	frame[7] = 0x00;
	frame[8] = message;
	frame[totalSize-1] = checksum(frame);
}

/** Send frame for serial port
 */
void sendFrame() {
	// cout<<"Sending frame...\n";
	Serial.write(frame, frameSize);
}

/** Print the frame that will be sent
 */
void printFrame() {
	cout<<"Frame: ";
	for (int i = 0; i < frameSize; ++i) {
		if (i != frameSize-1)
			cout<<hex(frame[i])<<", ";
		else
			cout<<hex(frame[i])<<endl;
	}
}

/** Get data from serial port
 */
bool getResponse() {
	int responseSize = 0;
	int serialCounter = 0;
	
	while(responseSize == 0){
		responseSize = Serial.available();
		if(serialCounter==100)
			return false;
		serialCounter++;
		delay(1);
	}

	serialCounter = 0;
	
	if(responseSize==8){
		for(int i=0; i<responseSize; i++){
			buffNodeData[i] = Serial.read();
		}
		responseSize = 0;

		while(responseSize == 0){
			responseSize = Serial.available();
			if(serialCounter==100)
				return false;
			serialCounter++;
			delay(1);
		}

		if(responseSize==2){
			buffNodeData[8]=Serial.read();
			buffNodeData[9]=Serial.read();
			// Compare the direction from which the data is been sent
			if(buffNodeData[5]==addr[1]){
				for(int i=0; i<10; i++){
					nodeData[i]=buffNodeData[i];
					cout<<hex(nodeData[i])<<" ";

				}
				cout<<endl;
				return true;
			}
			else return false;
		}
		else{
			for(int i=0; i<responseSize; i++)
				Serial.read();
			return false;
		}
	
	}
	else{
		for(int i=0; i<responseSize; i++)
			Serial.read();
		return false;
	}


}

char getData(char addr) {

	return 0;
}


/** Setup the ports of raspberry
 */
void setup(){
	Serial.begin(9600);
}

/** This function repeats itself infinitely 
 */
void loop() {
	// Get data from server and set into the house class
	string json = httpRequest("?function=getHomeLevel\\&home_id=1\\&home_password=myhome_pass");

	Document d;
	d.Parse<0>(json.c_str());

	int homeID, roomID, roomLevel, NODES, roomDL;
	bool success, roomControl, roomAutomatic;
	// bool mode;
	string message, homeName, roomName;

	success = d["success"].GetBool();
	message = d["message"].GetString();

	Home house;

	if (success) {
		// Get data from JSON document
		homeID = d["home"]["id"].GetInt();
		homeName = d["home"]["name"].GetString();
		// mode = d["home"]["mode"].GetBool();
		NODES = d["home"]["nodes"].GetInt();

		house.setHomeId(homeID);
		house.setHomeName(homeName);
		// house.setMode(mode);

		house.createNodes(NODES);
		house.printHomeData();

		char cRoom[] = "room_x";
		for (int i = 0; i < NODES; ++i) {
	    	cRoom[5] = (char) (i + '0');
	    	// cout<<cRoom<<endl;
	    	roomID = d["home"][cRoom]["id"].GetInt();
	    	roomDL = d["home"][cRoom]["DL"].GetInt();
			roomName = d["home"][cRoom]["name"].GetString();
			roomLevel = d["home"][cRoom]["level"].GetInt();
			roomControl = d["home"][cRoom]["control"].GetBool();
			roomAutomatic = d["home"][cRoom]["automatic"].GetBool();

			house.setNodes(i, roomID,roomDL, roomName, roomLevel, roomControl, roomAutomatic);
			// house.printNodeData(i);
		}
	}
	else {
		cout<<message<<endl;
	}
		  
	//Remove json file and return
	system(stringToChar("rm " + FILE_NAME));

	// Do everything of this if the control is done by the system 
	for(int i=0; i<NODES; i++){
		addr[1] = house.getNodeDL(i);
		
		// If the control of the node phisically is enabled for system control
		if(house.getNodeControl(i)){
			if(house.getNodeAutomatic(i)) {
				// Make the automatic
				cout<<"The room "<<house.getNodeName(i)<<" is in automatic mode"<<endl;
			}
			else{
				// Control made by the phone
				cout<<"The room "<<house.getNodeName(i)<<" is controlled by phone"<<endl;

				// Get the message according to the actual Node
				
				msg = house.getNodeLevel(i);
				
				// Generate Frame
				getFrame(msg, addr);
				// Show the frame to be sent
				printFrame();
				// Send Frame
				cout<<"Sending data to node #"<<i<<endl;
				sendFrame();
				// // Go to the next node address
				// addr[1]++;
				// // If we iterated through all the nodes then re-itialize and re-send data
				// if(addr[1] == NODES+1)
				// 	addr[1] = 0x01;
			}
		}
		else{
			cout<<"The room "<<house.getNodeName(i)<<" is disabled phisically"<<endl;
		}
		
		// Receive data: Switch State
		bool flag = false;
		int receiveDataCounter = 0;
		while(!flag){
			flag = getResponse();
			if(receiveDataCounter==10){
				flag=true;
				cout<<"Bad Response from Node "<<i<<endl;
			}
				
			receiveDataCounter++;
			delay(2);
		}
		// Post the data in server


		cout<<endl<<endl;
		delay(1);
	}
}

/** Main function
 */
int main (){
	setup();
	while(1){
		loop();
		delay(1);
	}
	return (0);
}