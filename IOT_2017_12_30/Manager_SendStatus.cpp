// (c) 2017-2018 Dan Saul, All Rights Reserved

#include "Manager.h"
#include "Pin.h"
#include <WiFiManager.h>

extern std::vector<Pin> pins;

void Manager::SendStatus()
{
	for (int i = 0; i < pins.size(); i++) pins[i].NotifyNetworkPacketStart();
	
	StaticJsonBuffer<UDP_TX_PACKET_MAX_SIZE> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["pVer"] = 1;
	root["type"] = "status";
	root["time"] = millis();
	root["MAC"] = WiFi.macAddress();
	
	JsonArray& pinsJSON = root.createNestedArray("pins");
	for (int i = 0; i < pins.size(); i++) {
		JsonObject &object = pinsJSON.createNestedObject();
		pins[i].PopulateStatusObject(object);
	}
	
	//Serial.printf("status @ %i\n", millis());
	udpSocket.beginPacket(cncServer, atoi(cncPort));
	root.printTo(udpSocket);
	//root.prettyPrintTo(Serial);
	udpSocket.print('\n');
	udpSocket.endPacket();
	
	for (int i = 0; i < pins.size(); i++) pins[i].NotifyNetworkPacketEnd();
	
}