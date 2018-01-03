// (c) 2017-2018 Dan Saul, All Rights Reserved

#include "Manager.h"
#include "Pin.h"
#include "PinOutputPrimary.h"
#include "PinButton.h"
#include "PinNetworkLED.h"
#include "PinOutputAuxilliary.h"
#include "PinOutputStatusLED.h"
#include <WiFiManager.h>
#include "ActionEnableID.h"

extern std::vector<std::shared_ptr<Pin>> pins;
std::vector<std::shared_ptr<Pin>> pins;

void Manager::DoSetup()
{
	auto buttonD1 = std::shared_ptr<PinButton>(new PinButton(this, "D1", D1, true));
	buttonD1->AddActionOnPressStart(std::shared_ptr<ActionEnableID>(new ActionEnableID(this, "D0")));
	
	pins.push_back(std::shared_ptr<PinOutputPrimary>(new PinOutputPrimary(this, "D0", D0, true)));
	pins.push_back(buttonD1);
	pins.push_back(std::shared_ptr<PinNetworkLED>(new PinNetworkLED(this, "esp8266_built_in", 2, false)));
	pins.push_back(std::shared_ptr<PinNetworkLED>(new PinNetworkLED(this, "D8", D8, true)));

	// Default Server Values
	// These are overriten by config.json
	
	strlcpy(cncServer, "", sizeof(cncServer));
	strlcpy(cncPort, "", sizeof(cncPort));
	strlcpy(cncSecret, "", sizeof(cncSecret));

	//Serial.printf("** %d\n",sizeof(cncServer));
	
	// Let the individual pins set themselves up.
	for (int i = 0; i < pins.size(); i++) {
		pins[i]->DoSetup();
	}

	DoSetupSPIFFS();
	DoSetupWiFiManager();
	DoSetupNetworkReceive();

	
}

int millisLastStatus = 0;
int statusResolution = 5000;

void Manager::DoLoop()
{
	
	// Let the pins do their thing before status is sent out.
	for (int i = 0; i < pins.size(); i++) {
		pins[i]->DoLoop();
	}
	
	
	// Every [statusResolution]ms (default 1000) we send the 
	// status of the pins and device to the controller. This 
	// is so that the controller has a fine grained view of 
	// what is happening in the system. My previous experience 
	// with the insteon system is that those only got an update
	// when either the device changed status or when you polled 
	// for the information, this leads to long periods of time 
	// where the device status is unknown.
	if (millis() >= (millisLastStatus + statusResolution)) {
		millisLastStatus = millis();
		
		SendStatus();
	}

	DoLoopNetworkReceive();
}

void Manager::EnableId(const char *id)
{
	
	for (int i = 0; i < pins.size(); i++) {
		if (0 == strcmp(id,pins[i]->id)) {
			pins[i]->DoEnable();
			break;
		}
	}
	
}

void Manager::DisableId(const char *id)
{
	
	for (int i = 0; i < pins.size(); i++) {
		if (0 == strcmp(id,pins[i]->id)) {
			pins[i]->DoDisable();
			break;
		}
	}
	
}

void Manager::Test()
{
	Serial.println("Manager::Test()");
}

