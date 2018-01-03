// (c) 2017-2018 Dan Saul, All Rights Reserved

#ifndef _ACTION_H_
#define _ACTION_H_

#include "Manager.h"

class Action {
	public:
	Manager* manager;
	
	Action(Manager * _manager);
	~Action();
	
	virtual void DoAction() = 0;
};

#endif