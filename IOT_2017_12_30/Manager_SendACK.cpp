// (c) 2017-2018 Dan Saul, All Rights Reserved

#include "Manager.h"
#include "Pin.h"
#include <WiFiManager.h>

extern std::vector<Pin> pins;

void Manager::SendACK(const char * uid)
{
	for (int i = 0; i < pins.size(); i++) pins[i].NotifyNetworkPacketStart();
	
	StaticJsonBuffer<UDP_TX_PACKET_MAX_SIZE> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["pVer"] = 1;
	root["type"] = "acknowledgment";
	root["time"] = millis();
	root["MAC"] = WiFi.macAddress();
	root["uid"] = uid;
	
	//Serial.printf("ack @ %i\n", millis());
	udpSocket.beginPacket(cncServer, atoi(cncPort));
	root.printTo(udpSocket);
	udpSocket.print('\n');
	udpSocket.endPacket();
	
	for (int i = 0; i < pins.size(); i++) pins[i].NotifyNetworkPacketEnd();
}