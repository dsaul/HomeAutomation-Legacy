// (c) 2017-2018 Dan Saul, All Rights Reserved

#include "Manager.h"
#include "Pin.h"
#include <WiFiManager.h>

std::vector<Pin> pins;

void Manager::DoSetup()
{
	pins.push_back(Pin(this, "0", D0, true, kPinUseCaseOutputPrimary));
	pins.push_back(Pin(this, "esp8266_built_in", 2, false, kPinUseCaseNetworkLED))

	// Default Server Values
	// These are overriten by config.json
	
	strlcpy(cncServer, "", sizeof(cncServer));
	strlcpy(cncPort, "", sizeof(cncPort));
	strlcpy(cncSecret, "", sizeof(cncSecret));

	//Serial.printf("** %d\n",sizeof(cncServer));
	
	// Let the individual pins set themselves up.
	for (int i = 0; i < pins.size(); i++) {
		pins[i].DoSetup();
	}
	
	
	pinMode(2, OUTPUT);

	DoSetupSPIFFS();
	DoSetupWiFiManager();
	
	udpSocket.begin(1234);

	digitalWrite(2, HIGH); // for some reason pin 2 has to be high instead of low to be off?
}

int millisLastStatus = 0;
int statusResolution = 1000;

void Manager::DoLoop()
{
	if (millis() >= (millisLastStatus + statusResolution)) {
		millisLastStatus = millis();
		
		SendStatus();
	}
	
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
			Serial.print("# MSG # ");
			Serial.print(type);
			Serial.print(" # ");
			
			if (0 == strcmp(type, "enableId")) {
				HandleEnableIdCMD(root);
			} else if (0 == strcmp(type, "disableId")) {
				HandleDisableIdCMD(root);
			}
			
			Serial.print("\n");
		}
		
		
		
	}
	
	
	
}

void Manager::EnableId(const char *id) {
	
	for (int i = 0; i < pins.size(); i++) {
		if (0 == strcmp(id,pins[i].id)) {
			pins[i].DoEnable();
			break;
		}
	}
	
}

void Manager::DisableId(const char *id) {
	
	for (int i = 0; i < pins.size(); i++) {
		if (0 == strcmp(id,pins[i].id)) {
			pins[i].DoDisable();
			break;
		}
	}
	
}










void Manager::SendStatus() {
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
	
	Serial.printf("status @ %i\n", millis());
	udpSocket.beginPacket(cncServer, atoi(cncPort));
	root.printTo(udpSocket);
	root.prettyPrintTo(Serial);
	udpSocket.print('\n');
	udpSocket.endPacket();
}

void Manager::SendACK(const char * uid) {
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
}

void Manager::SendNotify(const char * event, const char * dataKey, const char * dataValue) {
	SendNotifyTmpl(event, dataKey, dataValue);
}

template <class T> void Manager::SendNotifyTmpl(const char * event, const char * dataKey, T dataValue) {
	StaticJsonBuffer<UDP_TX_PACKET_MAX_SIZE> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["pVer"] = 1;
	root["type"] = "notify";
	root["time"] = millis();
	root["MAC"] = WiFi.macAddress();
	root["event"] = event;
	root[dataKey] = dataValue;
	
	//Serial.printf("notify @ %i\n", millis());
	udpSocket.beginPacket(cncServer, atoi(cncPort));
	root.printTo(udpSocket);
	udpSocket.print('\n');
	udpSocket.endPacket();
}

void Manager::Test()
{
	Serial.println("Manager::Test()");
}

void Manager::HandleEnableIdCMD(JsonObject& root) {
	
	const char * id = root["id"];
	Serial.print("id:");
	Serial.print(id);
	
	EnableId(id);
	
	
}

void Manager::HandleDisableIdCMD(JsonObject& root) {
	
	const char * id = root["id"];
	Serial.print("id:");
	Serial.print(id);
	
	DisableId(id);
	
}