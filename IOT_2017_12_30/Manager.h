// (c) 2017-2018 Dan Saul, All Rights Reserved

#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <vector>
#include <FS.h>			 //this needs to be first, or it all crashes and burns...
#include <ArduinoJson.h>
#include <WiFiUdp.h>

class Manager {
	/*
	** Variables
	*/
	
	public:
	
	char cncServer[40];
	char cncPort[6];
	char cncSecret[34];
	
	private:
	WiFiUDP udpSocket;
	char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet

	/*
	** Member Functions
	*/
	public:
	void DoSetup();
	void DoLoop();

	void EnableId(const char *id);
	void DisableId(const char *id);

	void SendStatus();
	void SendACK(const char *uid);
	void SendNotify(const char *event, const char *dataKey, const char *dataValue);
	
	void Test();

	private:
	template <class T> void SendNotifyTmpl(const char *event, const char *dataKey, T dataValue);
	void DoSetupSPIFFS();
	void DoSetupWiFiManager();
	void DoSetupNetworkReceive();

	void DoLoopNetworkReceive();
};

#endif