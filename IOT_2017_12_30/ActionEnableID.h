// (c) 2017-2018 Dan Saul, All Rights Reserved

#ifndef _ACTION_ENABLE_ID_H_
#define _ACTION_ENABLE_ID_H_

#include "Action.h"

class ActionEnableID : public Action {
	public:
	const char * id;
	
	ActionEnableID(Manager * _manager, const char * _id);
	~ActionEnableID();

	void DoAction() override;
};

#endif