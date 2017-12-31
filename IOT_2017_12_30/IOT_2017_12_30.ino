// (c) 2017-2018 Dan Saul, All Rights Reserved

#include "Manager.h"
#include "Pin.h"

Manager manager;

void setup()
{
	// Serial happens first so that we can get logging output.
	Serial.begin(115200);
	Serial.println();
	
	//Serial.println("hi");
	
	manager.DoSetup();
}

void loop()
{
	manager.DoLoop();
}