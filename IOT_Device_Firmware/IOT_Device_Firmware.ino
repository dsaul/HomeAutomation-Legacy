#include "common.h"
#include "Pin.h"
#include "Manager.h"

Manager manager;

// Default Server Values
// These are overriten by config.json
char cncServer[40];
char cncPort[6] = "";
char cncSecret[34] = "";


WiFiUDP _udpSocket;
char _packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet


//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
	Serial.println("Should save config");
	shouldSaveConfig = true;
}


void setup()
{
	// Serial happens first so that we can get logging output.
	Serial.begin(115200);
	Serial.println();
	
	manager.DoSetup();
	
	
	
	pinMode(2, OUTPUT);

	
	
	//clean FS, for testing
	//SPIFFS.format();

	//read configuration from FS json
	Serial.println("mounting FS...");

	if (SPIFFS.begin()) {
		Serial.println("mounted file system");
		if (SPIFFS.exists("/config.json")) {
			//file exists, reading and loading
			Serial.println("reading config file");
			File configFile = SPIFFS.open("/config.json", "r");
			if (configFile) {
				Serial.println("opened config file");
				size_t size = configFile.size();
				// Allocate a buffer to store contents of the file.
				std::unique_ptr<char[]> buf(new char[size]);

				configFile.readBytes(buf.get(), size);
				DynamicJsonBuffer jsonBuffer;
				JsonObject& json = jsonBuffer.parseObject(buf.get());
				json.printTo(Serial);
				if (json.success()) {
					Serial.println("\nparsed json");

					strcpy(cncServer, json["cncServer"]);
					strcpy(cncPort, json["cncPort"]);
					strcpy(cncSecret, json["cncSecret"]);

				} else {
					Serial.println("failed to load json config");
				}
			}
		}
	} else {
		Serial.println("failed to mount FS");
	}
	//end read



	// The extra parameters to be configured (can be either global or just in the setup)
	// After connecting, parameter.getValue() will get you the configured value
	// id/name placeholder/prompt default length
	WiFiManagerParameter custom_cnc_server("CncIP", "CNC Server", cncServer, 40);
	WiFiManagerParameter custom_cnc_port("CncPort", "CNC Port", cncPort, 6);
	WiFiManagerParameter custom_cnc_token("CncToken", "CNC Token", cncSecret, 32);

	//WiFiManager
	//Local intialization. Once its business is done, there is no need to keep it around
	WiFiManager wifiManager;
	
	//set config save notify callback
	wifiManager.setSaveConfigCallback(saveConfigCallback);

	//set static ip
	//wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

	Serial.print("WiFi MAC = ");
	Serial.println(WiFi.macAddress());
	
	//add all your parameters here
	wifiManager.addParameter(&custom_cnc_server);
	wifiManager.addParameter(&custom_cnc_port);
	wifiManager.addParameter(&custom_cnc_token);

	//reset settings - for testing
	//wifiManager.resetSettings();

	//set minimu quality of signal so it ignores AP's under that quality
	//defaults to 8%
	//wifiManager.setMinimumSignalQuality();
	
	//sets timeout until configuration portal gets turned off
	//useful to make it all retry or go to sleep
	//in seconds
	//wifiManager.setTimeout(120);

	//fetches ssid and pass and tries to connect
	//if it does not connect it starts an access point with the specified name
	//here	"AutoConnectAP"
	//and goes into a blocking loop awaiting configuration
	char ssid[32] = "IOT Device ";
	strcat(ssid, WiFi.macAddress().c_str());
	if (!wifiManager.autoConnect(ssid, "password")) {
		Serial.println("failed to connect and hit timeout");
		delay(3000);
		//reset and try again, or maybe put it to deep sleep
		ESP.reset();
		delay(5000);
	}

	//if you get here you have connected to the WiFi
	Serial.println("connected...yeey :)");

	//read updated parameters
	strcpy(cncServer, custom_cnc_server.getValue());
	strcpy(cncPort, custom_cnc_port.getValue());
	strcpy(cncSecret, custom_cnc_token.getValue());

	//save the custom parameters to FS
	if (shouldSaveConfig) {
		Serial.println("saving config");
		DynamicJsonBuffer jsonBuffer;
		JsonObject& json = jsonBuffer.createObject();
		json["cncServer"] = cncServer;
		json["cncPort"] = cncPort;
		json["cncSecret"] = cncSecret;

		File configFile = SPIFFS.open("/config.json", "w");
		if (!configFile) {
			Serial.println("failed to open config file for writing");
		}

		json.printTo(Serial);
		json.printTo(configFile);
		configFile.close();
		//end save
	}

	Serial.println("local ip");
	Serial.println(WiFi.localIP());


	
	

	_udpSocket.begin(1234);

	digitalWrite(2, HIGH); // for some reason pin 2 has to be high instead of low to be off?
}

int millisLastStatus = 0;
int statusResolution = 1000;

void loop() {
	
	
	
	if (millis() >= (millisLastStatus + statusResolution)) {
		millisLastStatus = millis();
		
		SendStatus();
	}
	
	
	
	
	
	
	
	// if there's data available, read a packet
	int packetSize = _udpSocket.parsePacket();
	//Serial.printf("packetSize: %i\n",packetSize);
	
	if (packetSize) {
		//Serial.print("Received packet of size ");
		//Serial.println(packetSize);
		//Serial.print("From ");
		//IPAddress remoteIp = _udpSocket.remoteIP();
		//Serial.print(remoteIp);
		//Serial.print(", port ");
		//Serial.println(_udpSocket.remotePort());

		// read the packet into packetBufffer
		int len = _udpSocket.read(_packetBuffer, UDP_TX_PACKET_MAX_SIZE);
		if (len > 0) {
			_packetBuffer[len] = 0;
		}
		//Serial.println("Contents:");
		//Serial.println(_packetBuffer);
		
		
		StaticJsonBuffer<UDP_TX_PACKET_MAX_SIZE> jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(_packetBuffer);
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
				HandleEnableIndexCMD(root);
			} else if (0 == strcmp(type, "disableId")) {
				HandleDisableIndexCMD(root);
			}
			
			Serial.print("\n");
		}
		
		
		
	}

	

	
	
	//delay(50);

	
}














































void SendStatus() {
	StaticJsonBuffer<UDP_TX_PACKET_MAX_SIZE> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["protocolVersion"] = 1;
	root["type"] = "status";
	root["millis"] = millis();
	root["MAC"] = WiFi.macAddress();
	
	//Serial.printf("heartbeat @ %i\n", millis());
	_udpSocket.beginPacket(cncServer, atoi(cncPort));
	root.printTo(_udpSocket);
	_udpSocket.print('\n');
	_udpSocket.endPacket();
}

void SendACK(const char * uid) {
	StaticJsonBuffer<UDP_TX_PACKET_MAX_SIZE> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["protocolVersion"] = 1;
	root["type"] = "acknowledgment";
	root["millis"] = millis();
	root["MAC"] = WiFi.macAddress();
	root["uid"] = uid;
	
	//Serial.printf("heartbeat @ %i\n", millis());
	_udpSocket.beginPacket(cncServer, atoi(cncPort));
	root.printTo(_udpSocket);
	_udpSocket.print('\n');
	_udpSocket.endPacket();
}

template <class T> void SendNotify(const char * event, const char * dataKey, T dataValue) {
	StaticJsonBuffer<UDP_TX_PACKET_MAX_SIZE> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["protocolVersion"] = 1;
	root["type"] = "notify";
	root["millis"] = millis();
	root["MAC"] = WiFi.macAddress();
	root["event"] = event;
	root[dataKey] = dataValue;
	
	//Serial.printf("heartbeat @ %i\n", millis());
	_udpSocket.beginPacket(cncServer, atoi(cncPort));
	root.printTo(_udpSocket);
	_udpSocket.print('\n');
	_udpSocket.endPacket();
}










void HandleEnableIndexCMD(JsonObject& root) {
	
	const char * id = root["id"];
	Serial.print("id:");
	Serial.print(id);
	
	manager.EnableId(id);
	
	
}

void HandleDisableIndexCMD(JsonObject& root) {
	
	const char * id = root["id"];
	Serial.print("id:");
	Serial.print(id);
	
	manager.DisableId(id);
	
}



