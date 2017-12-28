#include "common.h"
#include "Pin.h"
#include "Manager.h"

Manager manager;

void setup()
{
	// Serial happens first so that we can get logging output.
	Serial.begin(115200);
	Serial.println();
	
	manager.DoSetup();
}



void loop()
{
	manager.DoLoop();
}








