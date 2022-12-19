/*
 Name:		gardRobot.ino
 Created:	11/9/2022 7:13:03 PM
 Author:	ipewz
*/
#include "WiFiControler.h"
#include <WebSocketsServer.h>
#include <WebSockets.h>
#include "Camera.h"
#include "Control.h"
#include "Motor.h"
#include "IMU.h"


#pragma region define
#define MOTOR_A1  12
#define MOTOR_A2  13
#define MOTOR_B1  3
#define MOTOR_B2  2
#define SERVO_MOTOR  999


#define MY_SSID	"*****"
#define MY_PASS	"*****"
#pragma endregion 

WiFiControler wiFiControler;

IMU* imu;
Motor* motorA, * motorB;
Control* control;
Camera* cam;
WebSocketsServer webSocket(82);
float oldAngle = 0, newAngle=0;
//WiFiMulti wiFiMulti;


void hexdump(const void* mem, uint32_t len, uint8_t cols = 16) {
    const uint8_t* src = (const uint8_t*)mem;
    Serial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
    for (uint32_t i = 0; i < len; i++) {
        if (i % cols == 0) {
            Serial.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
        }
        Serial.printf("%02X ", *src);
        src++;
    }
    Serial.printf("\n");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    //extern WebSocketsServer* webSocket;

    switch (type) {
    case WStype_DISCONNECTED:{   
        Serial.printf("[%u] Disconnected!\n", num); 
    } break;
    case WStype_CONNECTED:{
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        webSocket.sendTXT(num, "Connected");
    } break;
    case WStype_TEXT:{
        Serial.printf("[%u] get Text: %s\n", num, payload);
        // convert payload data to 2 intgers

        int rightValue = 0, leftValue = 0, i = 0;
        while (payload[i] != ',') {
            rightValue = rightValue * 10 + (payload[i] - '0');
            i++;
        }
        i++;
        while (payload[i] != '\0') {
            leftValue = leftValue * 10 + (payload[i] - '0');
            i++;
        }
        control->move(rightValue, leftValue);
    } break;

    // send message to client
    // webSocket.sendTXT(num, "message here");
    // send data to all connected clients
    // webSocket.broadcastTXT("message here");
//      break;
    case WStype_BIN: { Serial.printf("[%u] get binary length: %u\n", num, length);
    hexdump(payload, length);
    } break;
    // send message to client
    // webSocket.sendBIN(num, payload, length);
   
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
        break;
    }

}



//---------------------------

void setup() {
    Serial.begin(115200);
    wiFiControler.init();
													
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    
    //--------------------------------------

    imu = new IMU();
    imu->init();
    motorA = new Motor(MOTOR_A1, MOTOR_A2, false);
    motorA->init();                                    
    motorB = new Motor(MOTOR_B1, MOTOR_B2, false);
    motorB->init();
    control = new Control(imu, motorA, motorB);
    control->init();
    cam = new Camera(MY_SSID, MY_PASS, control);
    cam->init();
   // wiFiControler.startMdns();

}

// the loop function runs over and over again until power down or reset
void loop() {
    webSocket.loop();
   
   newAngle = control->tiltAngle();
    if(oldAngle != newAngle){
        webSocket.broadcastTXT("" + String(int(newAngle)));
        oldAngle = newAngle;
	}
	
    control->keepSteady();
}

