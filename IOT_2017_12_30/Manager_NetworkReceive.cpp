// (c) 2017-2018 Dan Saul, All Rights Reserved

#include "Manager.h"

void Manager::DoSetupNetworkReceive()
{
	udpSocket.begin(1234);
}

void Manager::DoLoopNetworkReceive()
{
	// if there's data available, read a packet
	int packetSize = udpSocket.parsePacket();
	//Serial.printf("packetSize: %i\n",packetSize);
	
	if (packetSize) {
		//Serial.print("Received packet of size ");
		//Serial.println(packetSize);
		//Serial.print("From ");
		//IPAddress remoteIp = udpSocket.remoteIP();
		//Serial.print(remoteIp);
		//Serial.print(", port ");
		//Serial.println(udpSocket.remotePort());

		// read the packet into packetBufffer
		int len = udpSocket.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
		if (len > 0) {
			packetBuffer[len] = 0;
		}
		//Serial.println("Contents:");
		//Serial.println(packetBuffer);
		
		
		StaticJsonBuffer<UDP_TX_PACKET_MAX_SIZE> jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(packetBuffer);
		if (!root.success()) {
			Serial.println("Parsing of packet failed!");
		} else {
			//Serial.println("Parsing of packet succeeded!");
			
			const char* uid = root["uid"];
			SendACK(uid);
			
			
			const char* type = root["type"];
			Serial.print("< MSG[");
			Serial.print(type);
			Serial.print("] ");
			
			if (0 == strcmp(type, "enableId")) {
				const char * id = root["id"];
				Serial.print("id[");
				Serial.print(id);
				Serial.print("]");
				
				EnableId(id);
			} else if (0 == strcmp(type, "disableId")) {
				const char * id = root["id"];
				Serial.print("id[");
				Serial.print(id);
				Serial.print("]");
				
				DisableId(id);
			}
			
			Serial.print("\n");
		}
		
		
		
	}
}