/*// 
// 
// 

#include "WiFiControler.h"

void WiFiControlerClass::init()
{


}


WiFiControlerClass WiFiControler;
			*/

            // 
            // 
            // 

#include "WiFiControler.h"

WiFiControler::WiFiControler() {
    shouldSaveConfig = 0;
    enterConfigPortel = 0;
	
    //mdnsName  allocate memory for 20 charector
   //mdnsName = (char*)malloc(40 * sizeof(char));     //new char[40]; 
   //mdnsName = "";
    //wifiAPs = (WiFiAP*)malloc(20);
}

WiFiControler::~WiFiControler() {
    //free(mdnsName);
    //free(wifiAPs);
}

//callback notifying us of the need to save config
void WiFiControler::saveConfigCallback() {
    Serial.println("Should save config");
    shouldSaveConfig = true;
}

void WiFiControler::init() {
    pinMode(RESET_BUTTON, INPUT_PULLUP);

    //SPIFFS.format();     //clean FS, for testing
    readConfig();

    /*
    The extra parameters to be configured(can be either global or just in the setup)
    After connecting, parameter.getValue() will get you the configured value
    id/name placeholder/prompt default length
    */
    WiFiManagerParameter custom_mdnsName("mdnsName", "mdns Name", mdnsName, 40);

    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;

    //set config save notify callback
    wifiManager.setSaveConfigCallback([&] {this->saveConfigCallback(); });
   
    //custom_mdnsName.getValue();   //delete me

    //add all your parameters here
    wifiManager.addParameter(&custom_mdnsName);
    //reset settings - for testing
    //wifiManager.resetSettings();
    Serial.println("init l:68");

    if (digitalRead(RESET_BUTTON) < 1) {
        Serial.println("init l: 71");

        if (!wifiManager.startConfigPortal("ESP_AutoConnectAP"/*, "password"*/)) {
            Serial.println("init l: 74");

            Serial.println("failed to connect and hit timeout");
            delay(3 * 1000);
            //reset and try again, or maybe put it to deep sleep
            ESP.restart();
            delay(5 * 1000);
        }
    }
    Serial.println("init l: 83");

    /*
    fetches ssid and pass and tries to connect
    if it does not connect it starts an access point with the specified name
    here  "AutoConnectAP"
    and goes into a blocking loop awaiting configuration
    */
    if (!wifiManager.autoConnect("ESP_AutoConnectAP"/*, "password"*/)) {
        Serial.println("init l: 94");

        Serial.println("failed to connect and hit timeout");
        delay(3 * 1000);
        //reset and try again, or maybe put it to deep sleep
        ESP.restart();
        delay(5 * 1000);
    }
    Serial.println("init l: 102");

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");

    //read updated parameters
    strcpy(mdnsName, custom_mdnsName.getValue());

    //Serial.println("The values in the file are: ");
    //Serial.println("\tmdnsName : " + String(mdnsName));

    //save the custom parameters to FS
    if (shouldSaveConfig) { savingConfig(); }

    Serial.println("local ip: " + WiFi.localIP().toString());
}

void WiFiControler::startMdns() {
    Serial.println("line 117");

    if (mdnsName == NULL ){
        Serial.println("line 120");

        return;
        Serial.println("line 124");

    }
    if (mdnsName[0] == '\0') {
        Serial.println("line 127");

        return;
    }                            
    Serial.println("line 131");

    if (!MDNS.begin(mdnsName)) {
        Serial.println("Error setting up MDNS responder!");
    }
    else {
        
        MDNS.addService("http", "tcp", 80);
        Serial.println("mDNS responder started");
        Serial.println("mDNS address: http://" + String(mdnsName) + ".local");
    }
}

void WiFiControler::savingConfig() {
    Serial.println("saving config");

    DynamicJsonDocument json(1024);

    json["mdnsName"] = mdnsName;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) { Serial.println("failed to open config file for writing"); }

    serializeJson(json, Serial);
    serializeJson(json, configFile);

    configFile.close();
}

void WiFiControler::readConfig() {
    //read configuration from FS json
    Serial.println("mounting FS...");

    if (SPIFFS.begin()) {
        Serial.println("mounted file system");
        if (SPIFFS.exists("/config.json")) {
            //file exists, reading and loading
            Serial.println("reading config file");
            File configFile = SPIFFS.open("/config.json", "r");
            if (configFile) {
                Serial.println("opened config file");
                size_t size = configFile.size();
                // Allocate a buffer to store contents of the file.
                std::unique_ptr<char[]> buf(new char[size]);

                configFile.readBytes(buf.get(), size);

                DynamicJsonDocument json(1024);
                auto deserializeError = deserializeJson(json, buf.get());
                serializeJson(json, Serial);
                if (!deserializeError) {
                    Serial.println("\nparsed json");
                    strcpy(mdnsName, json["mdnsName"]);
                }
                else { Serial.println("failed to load json config"); }
                configFile.close();
            }
        }
        else { Serial.println("\"config.json\" not exists!"); }
    }
    else { Serial.println("failed to mount FS"); }
}


void WiFiControler::checkIfResetAndConigButtonIsPress() {
    if (digitalRead(RESET_BUTTON) < 1) {
        for (size_t i = 0; i < 6; i++) {
            // analogWrite(LED, 100 * (i % 2));
            delay(500);
        }
        if (digitalRead(RESET_BUTTON) < 1) {
            ESP.restart();
        }
    }
}

/*
void MQTTConnect() {
    mqttClient.setUsernamePassword("username", "password");
    mqttClient.setId("esp01");

    if (
        (mqtt_server[0] == '(') ||
        (mqtt_server[0] == ':') ||
        (mqtt_server[0] == '.') ||
        (mqtt_server[1] == '.') ||
        (mqtt_server[2] == '.') ||
        (mqtt_server[3] == '.')
        )
    {     //is ip addras
        mqtt_serverIP.fromString(mqtt_server);
        Serial.print(mqtt_serverIP.toString());
        Serial.print("Try connecting to the MQTT broker: " + String(mqtt_serverIP.toString()));
        while (!mqttClient.connect(mqtt_serverIP)) {
            Serial.print("\nMQTT connection failed! Error code = " + String(mqttClient.connectError()));
            delay(5000);

            //Press the button for 3 sec to enter  ConfigPortal
            checkIfResetAndConigButtonIsPress();
        }
    }
    else {   //is path addras
        Serial.print("Try connecting to the MQTT broker: " + String(mqtt_server));
        while (!mqttClient.connect(mqtt_server, atoi(mqtt_port))) {
            Serial.print("\nMQTT connection failed! Error code = " + String(mqttClient.connectError()));
            delay(5000);

            //Press the button for 3 sec to enter  ConfigPortal
            checkIfResetAndConigButtonIsPress();
        }
    }
    Serial.println("You're connected to the MQTT broker!");
    Serial.print("Subscribing to topic: "); Serial.print(String(mqtt_topic));

    mqttClient.subscribe(mqtt_topic);
    //mqttClient.unsubscribe(mqtt_topic);

    Serial.println("Waiting for messages on topic: "); Serial.println(String(mqtt_topic));
}
             */


/////
/*
nit l:68
init l: 71
*wm:[2] Starting Config Portal
*wm:[2] Disabling STA
*wm:[2] Enabling AP
*wm:[1] StartAP with SSID:  ESP_AutoConnectAP
*wm:[2] AP has anonymous access!
*wm:[1] AP IP address: 192.168.4.1
*wm:[1] Starting Web Portal
*wm:[2] HTTP server started
*wm:[2] WiFi Scan ASYNC started
*wm:[2] Config Portal Running, blocking, waiting for clients...
*wm:[2] NUM CLIENTS:  0
*wm:[2] NUM CLIENTS:  0
*wm:[2] <- Request redirected to captive portal
*wm:[2] <- HTTP Root
*wm:[2] WiFi Scan ASYNC started
*wm:[2] NUM CLIENTS:  0
*wm:[2] <- Request redirected to captive portal
*wm:[2] <- HTTP Root
*wm:[2] WiFi Scan ASYNC started
*wm:[2] <- Handle update
*wm:[2] <- HTTP Root
*wm:[2] <- Request redirected to captive portal
*wm:[2] <- HTTP Wifi
*wm:[2] WiFi Scan SYNC started
*wm:[2] WiFi Scan completed in 4703 ms
*wm:[1] 1 networks found
*wm:[2] AP:  -56 ip1
*wm:[2] NUM CLIENTS:  1
*wm:[2] <- HTTP WiFi save
*wm:[2] Parameters
*wm:[2] --------------------
*wm:[2] mdnsName: Ghgh
*wm:[2] --------------------
*wm:[2] processing save
*wm:[2] Connecting as wifi client...
*wm:[2] setSTAConfig static ip not set, skipping
*wm:[1] Connecting to NEW AP: ip1
*wm:[1] connectTimeout not set, ESP waitForConnectResult...
*wm:[2] Connection result: WL_CONNECTED
*wm:[1] Connect to new AP [SUCCESS]
*wm:[1] Got IP Address:
*wm:[1] 10.0.0.8
*wm:[2] [CB] _savewificallback calling
Should save config
*wm:[2] shutdownConfigPortal
*wm:[0] [ERROR] disconnect configportal - softAPdisconnect FAILED
*wm:[2] restoring usermode STA
*wm:[2] wifi status: WL_CONNECTED
*wm:[2] wifi mode: STA
*wm:[2] configportal closed
*wm:[1] config portal exiting
init l: 83
*wm:[1] AutoConnect
*wm:[2] ESP32 event handler enabled
*wm:[1] AutoConnect: ESP Already Connected
*wm:[2] setSTAConfig static ip not set, skipping
*wm:[1] AutoConnect: SUCCESS
*wm:[2] Connected in 15 ms
*wm:[1] STA IP Address: 10.0.0.8
connected...yeey :)
The values in the file are:
    mdnsName : Ghgh
saving config
{"mdnsName":"Ghgh"}local ip
10.0.0.8
MPU c-tor
MPU c-tor end
MPU init
Initializing I2C devices...
Testing device connections...
MPU6050 connection successful
Initializing DMP...
>......>......-4202.00000,	-415.00000,	1366.00000,	239.00000,	15.00000,	-35.00000

Enabling DMP...

MPU init end
[E][camera.c:1113] camera_probe(): Detected camera not supported.
[E][camera.c:1379] esp_camera_init(): Camera probe failed with error 0x20004

*/