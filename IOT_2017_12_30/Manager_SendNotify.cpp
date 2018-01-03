// (c) 2017-2018 Dan Saul, All Rights Reserved

#include "Manager.h"
#include "Pin.h"
#include <WiFiManager.h>

extern std::vector<std::shared_ptr<Pin>> pins;

void Manager::SendNotify(const char * event, const char * dataKey, const char * dataValue) {
	SendNotifyTmpl(event, dataKey, dataValue);
}

template <class T> void Manager::SendNotifyTmpl(const char * event, const char * dataKey, T dataValue) {
	
	for (int i = 0; i < pins.size(); i++) pins[i]->NotifyNetworkPacketStart();
	
	StaticJsonBuffer<UDP_TX_PACKET_MAX_SIZE> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["pVer"] = 1;
	root["type"] = "notify";
	root["time"] = millis();
	root["MAC"] = WiFi.macAddress();
	root["event"] = event;
	root[dataKey] = dataValue;
	
	Serial.printf("> notify @ %i\n", millis());
	udpSocket.beginPacket(cncServer, atoi(cncPort));
	root.printTo(udpSocket);
	udpSocket.print('\n');
	udpSocket.endPacket();
	
	for (int i = 0; i < pins.size(); i++) pins[i]->NotifyNetworkPacketEnd();
}