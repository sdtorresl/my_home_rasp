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

using namespace std;

string buffer;
string command;

int frameSize = 10;
char* frame = new char[frameSize];
const int NODES = 3;
char addr[] = {0x50, 0x01};

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

/** Get command from serial port
 */
string getResponse() {
	string cmd = "";
	bool begin = false;
	bool cmdRecognized = false;

	while(Serial.available() && !cmdRecognized) {
		buffer = Serial.read();
		
		//'/' is the begin of the command
		if (buffer.compare("/") == 0) 
			begin = true;

		if (begin) {
			cmd = cmd + buffer;

			// The ':' is a flag for recognize the command
			if (buffer.compare(":") == 0) {
				//Remove # from command
				char chars[] = ":/";
				cmd.erase(remove(cmd.begin(), cmd.end(), chars[0]), cmd.end());
				cmd.erase(remove(cmd.begin(), cmd.end(), chars[1]), cmd.end());

				cmdRecognized = true;
			}
		}
	}
	
	if (cmd.compare("") == 0) {
		cmd = "Can't get command";
	}

	return cmd;
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
void getFrame(char message[], char addr[], int size){
	size--;
	int totalSize = size + 9;
	char dh = addr[0];
	char dl = addr[1];

	//Resize the new frame array
	char* newFrame = new char[totalSize];
    memcpy(newFrame, frame, frameSize * sizeof(char));
    
    frameSize = totalSize;
    //Delete old array
    delete [] frame;
    //Swap pointers and new size
    frame = newFrame;
    
    frame[0] = 0x7E;
    frame[1] = 0x00;
    frame[2] = totalSize - 4;
	frame[3] = 0x01;
	frame[4] = 0x01;
	frame[5] = dh;
	frame[6] = dl;
	frame[7] = 0x00;

	for (int i = 8; i < totalSize-1; ++i) {
		frame[i] = message[i-8];
	}

	frame[totalSize-1] = checksum(frame);
}

/** Send frame for serial port
 */
void sendFrame() {
	cout<<"Sending frame...\n";
	Serial.write(frame, frameSize);
	delay(100);
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

/** Setup the ports of raspberry
 */
void setup(){
	Serial.begin(9600);
}

/** This function repeats itself infinitely 
 */
void loop() {
	char msg[] = {"/490:"};
	
	string response = "none";

	for (int i = 0; i < NODES; ++i) {
		addr[1] = i;

		getFrame(msg, addr, sizeof(msg));
		printFrame();
		sendFrame();
		
		response = getResponse();
		cout<<"Response: "<<response<<endl<<endl;
	}
}

/** Main function
 */
int main (){
	setup();
	while(1){
		loop();
	}
	return (0);
}