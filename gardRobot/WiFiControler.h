/*// WiFiControler.h

#ifndef _WIFICONTROLER_h
#define _WIFICONTROLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class WiFiControlerClass
{
 protected:


 public:
	void init();
};

extern WiFiControlerClass WiFiControler;

#endif
*/

// WiFiControler.h

#ifndef _WIFICONTROLER_h
#define _WIFICONTROLER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

//#include <WiFi.h>
//#include <WiFiMulti.h> 
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <FS.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <ESPmDNS.h>



#define RESET_BUTTON 16
//#define LED 4

	 /*
   struct WiFiAP {
	   char ssid[20];
	   char password[20];
   };

		 */

class WiFiControler {
public:

	WiFiClient wifiClient;
	// IPAddress mqtt_serverIP;// (192, 168, 150, 138);

//	 WiFiAP* wifiAPs;
	//char* mdnsName;
	char mdnsName[40];
	bool shouldSaveConfig, enterConfigPortel;

	WiFiControler();
	~WiFiControler();

	void init();
	void savingConfig();
	void checkIfResetAndConigButtonIsPress();
	void saveConfigCallback();
	void readConfig();
	void startMdns();
};
#endif

