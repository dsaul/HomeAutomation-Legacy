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
#include "ActionToggleID.h"
#include "ActionDisableID.h"

extern std::vector<std::shared_ptr<Pin>> pins;
std::vector<std::shared_ptr<Pin>> pins;

void Manager::OnSetup()
{
	pins.push_back(std::shared_ptr<PinOutputPrimary>(new PinOutputPrimary(this, "D0", D0, true)));
	
	auto buttonD1 = std::shared_ptr<PinButton>(new PinButton(this, "D1", D1, true));
	buttonD1->AddActionOnPressStart(std::shared_ptr<ActionToggleID>(new ActionToggleID(this, "D0")));
	pins.push_back(buttonD1);
	
	auto buttonD2 = std::shared_ptr<PinButton>(new PinButton(this, "D2", D2, true));
	buttonD2->AddActionOnPressStart(std::shared_ptr<ActionEnableID>(new ActionEnableID(this, "D0")));
	pins.push_back(buttonD2);
	
	auto buttonD3 = std::shared_ptr<PinButton>(new PinButton(this, "D3", D3, true));
	buttonD3->AddActionOnPressStart(std::shared_ptr<ActionDisableID>(new ActionDisableID(this, "D0")));
	pins.push_back(buttonD3);
	
	
	
	
	pins.push_back(std::shared_ptr<PinNetworkLED>(new PinNetworkLED(this, "esp8266_built_in", 2, false)));
	pins.push_back(std::shared_ptr<PinOutputStatusLED>(new PinOutputStatusLED(this, "D8", D8, true, "D0")));

	// Default Server Values
	// These are overriten by config.json
	
	strlcpy(cncServer, "", sizeof(cncServer));
	strlcpy(cncPort, "", sizeof(cncPort));
	strlcpy(cncSecret, "", sizeof(cncSecret));

	//Serial.printf("** %d\n",sizeof(cncServer));
	
	// Let the individual pins set themselves up.
	for (int i = 0; i < pins.size(); i++) {
		pins[i]->OnSetup();
	}

	DoSetupSPIFFS();
	DoSetupWiFiManager();
	DoSetupNetworkReceive();

	
}

int millisLastStatus = 0;
int statusResolution = 5000;

void Manager::OnLoop()
{
	
	// Let the pins do their thing before status is sent out.
	for (int i = 0; i < pins.size(); i++) {
		pins[i]->OnLoop();
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

void Manager::OnEnableId(const char *id)
{
	SendNotify("OnEnableId", "id", id);
	
	for (int i = 0; i < pins.size(); i++) {
		pins[i]->OnEnableId(id);
	}
}

void Manager::OnDisableId(const char *id)
{
	SendNotify("OnDisableId", "id", id);
	
	for (int i = 0; i < pins.size(); i++) {
		pins[i]->OnDisableId(id);
	}
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

void Manager::ToggleId(const char *id)
{
	
	for (int i = 0; i < pins.size(); i++) {
		if (0 == strcmp(id,pins[i]->id)) {
			pins[i]->DoToggle();
			break;
		}
	}
	
}




void Manager::Test()
{
	Serial.println("Manager::Test()");
}

