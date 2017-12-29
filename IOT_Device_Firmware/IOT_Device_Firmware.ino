
#include "Manager.h"
#include "Pin.h"

extern Manager manager;
Manager manager;
extern Pin pins[] = {
	Pin("0", D0, kPinUseCaseOutputPrimary)};
extern int pinsCount = sizeof(pins) / sizeof(*pins);

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