// (c) 2017-2018 Dan Saul, All Rights Reserved

#include "ActionDisableID.h"

ActionDisableID::ActionDisableID(Manager * _manager, const char * _id)
	: Action(_manager)
{
	id = _id;
}

ActionDisableID::~ActionDisableID()
{
	
}

void ActionDisableID::DoAction()
{
	manager->DisableId(id);
}