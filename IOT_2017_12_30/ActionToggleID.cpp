// (c) 2017-2018 Dan Saul, All Rights Reserved

#include "ActionToggleID.h"

ActionToggleID::ActionToggleID(Manager * _manager, const char * _id)
	: Action(_manager)
{
	id = _id;
}

ActionToggleID::~ActionToggleID()
{
	
}

void ActionToggleID::DoAction()
{
	manager->ToggleId(id);
}