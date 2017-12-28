#include "Manager.h"


//
// Configuration
//
Pin pins[] = {
	Pin("0", D0, kPinUseCaseOutputPrimary)
};




Manager::Manager()
{
	
}

Manager::~Manager()
{
	
}

void Manager::DoSetup()
{
	// Let the individual pins set themselves up.
	for (int i = 0; i < sizeof(pins) / sizeof(*pins); i++) {
		pins[i].DoSetup();
	}
}

void Manager::EnableId(const char *id) {
	
	for (int i = 0; i < sizeof(pins) / sizeof(*pins); i++) {
		if (0 == strcmp(id,pins[i].id)) {
			pins[i].DoEnable();
			break;
		}
	}
	
}

void Manager::DisableId(const char *id) {
	
	for (int i = 0; i < sizeof(pins) / sizeof(*pins); i++) {
		if (0 == strcmp(id,pins[i].id)) {
			pins[i].DoDisable();
			break;
		}
	}
	
}