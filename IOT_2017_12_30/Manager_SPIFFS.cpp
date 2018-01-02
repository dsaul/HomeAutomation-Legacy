// (c) 2017-2018 Dan Saul, All Rights Reserved

#include "Manager.h"

void Manager::DoSetupSPIFFS()
{
	//clean FS, for testing
	//SPIFFS.format();

	//read configuration from FS json
	Serial.println("i mounting FS...");

	if (SPIFFS.begin()) {
		Serial.println("i mounted file system");
		if (SPIFFS.exists("/config.json")) {
			//file exists, reading and loading
			Serial.println("i reading config file");
			File configFile = SPIFFS.open("/config.json", "r");
			if (configFile) {
				Serial.println("i opened config file");
				size_t size = configFile.size();
				// Allocate a buffer to store contents of the file.
				std::unique_ptr<char[]> buf(new char[size]);

				configFile.readBytes(buf.get(), size);
				DynamicJsonBuffer jsonBuffer;
				JsonObject& json = jsonBuffer.parseObject(buf.get());
				Serial.print("i stored json: ");
				json.printTo(Serial);
				Serial.print("\n");
				if (json.success()) {
					Serial.println("i parsed json");

					strlcpy(cncServer, json["cncServer"], sizeof(cncServer));
					strlcpy(cncPort, json["cncPort"], sizeof(cncPort));
					strlcpy(cncSecret, json["cncSecret"], sizeof(cncSecret));

				} else {
					Serial.println("e failed to load json config");
				}
			}
		}
	} else {
		Serial.println("i failed to mount FS");
	}
	//end read
}