#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "common.h"
#include "Pin.h"


class Manager {
	public:
		

		Manager();
		~Manager();

		void DoSetup();

		void EnableId(const char *id);
		void DisableId(const char *id);
};

#endif