#include "Manager.h"
#include <WiFiManager.h>

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
	Serial.println("Should save config");
	shouldSaveConfig = true;
}

void Manager::DoSetupWiFiManager()
{
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
	strlcpy(cncServer, custom_cnc_server.getValue(),sizeof(cncServer));
	strlcpy(cncPort, custom_cnc_port.getValue(), sizeof(cncPort));
	strlcpy(cncSecret, custom_cnc_token.getValue(), sizeof(cncSecret));

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
}