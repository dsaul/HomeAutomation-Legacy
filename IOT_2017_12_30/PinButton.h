// (c) 2017-2018 Dan Saul, All Rights Reserved

#ifndef _PIN_BUTTON_H_
#define _PIN_BUTTON_H_

#include "Pin.h"
#include "Action.h"

class PinButton : public Pin {
	public:
	bool isPullDown;
	
	PinButton(Manager * _manager, const char * _id, uint8_t _pinNumber, bool _isPullDown);
	~PinButton();

	void DoSetup() override;
	void DoLoop() override;
	void DoEnable() override;
	void DoDisable() override;
	void NotifyNetworkPacketStart() override;
	void NotifyNetworkPacketEnd() override;
	void PopulateStatusObject(JsonObject &object) override;
	
	void AddActionOnPressStart(std::shared_ptr<Action> _new);
	
	private:
	bool wasPressed;
	std::vector<std::shared_ptr<Action>> actionsOnPressStart;
};

#endif