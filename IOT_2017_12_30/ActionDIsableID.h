// (c) 2017-2018 Dan Saul, All Rights Reserved

#ifndef _ACTION_DISABLE_ID_H_
#define _ACTION_DISABLE_ID_H_

#include "Action.h"

class ActionDisableID : public Action {
	public:
	const char * id;
	
	ActionDisableID(Manager * _manager, const char * _id);
	~ActionDisableID();

	void DoAction() override;
};

#endif