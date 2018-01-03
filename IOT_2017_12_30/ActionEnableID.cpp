// (c) 2017-2018 Dan Saul, All Rights Reserved

#import "ActionEnableID.h"

ActionEnableID::ActionEnableID(Manager * _manager, const char * _id)
	: Action(_manager)
{
	id = _id;
}

ActionEnableID::~ActionEnableID()
{
	
}

void ActionEnableID::DoAction()
{
	//Serial.println("DoAction()");

	manager->EnableId(id);
}