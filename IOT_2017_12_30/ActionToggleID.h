// (c) 2017-2018 Dan Saul, All Rights Reserved

#ifndef _ACTION_TOGGLE_ID_H_
#define _ACTION_TOGGLE_ID_H_

#include "Action.h"

class ActionToggleID : public Action {
	public:
	const char * id;
	
	ActionToggleID(Manager * _manager, const char * _id);
	~ActionToggleID();

	void DoAction() override;
};

#endif