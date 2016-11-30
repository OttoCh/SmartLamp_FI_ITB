//ID: xxxxx911
#include <easyMesh.h>
//#include <SimpleList.h>
//#include <ArduinoJson.h>

#define MESH_PREFIX "LampuSelasarFisika"
#define MESH_PASSWORD "labiot2016"
#define MESH_PORT 5555

easyMesh mesh;
//deteksi LDR, bila dibawah nilai tertentu broadcast perintah turn on

const int noon = 500; //
unsigned long updateTime = 3000;  //15 menit
unsigned long last = millis();   //agar saat menyala langsung membaca
unsigned long now = millis()+updateTime+1000;

void setup() {
  Serial.begin(115200);
  Serial.println("begin");
  mesh.setDebugMsgTypes(ERROR|STARTUP);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, MESH_PORT);
  mesh.setReceiveCallback(&receivedCallback);
  mesh.setNewConnectionCallback( &newConnectionCallback );
}

void loop() {  
  mesh.update();
  //Serial.println(analogRead(A0));
  if(now-last > updateTime) {
    last = millis();
    sendBroadcast();
  }
  now = millis();
  //delay(200);
}

bool readADC() {
  if(analogRead(A0)<noon) { //sudah malam
    return true;
  }
  else return false;
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %d msg=%s\n", from, msg.c_str());
  //delay(1000);
  if(msg=="A") {
    sendBroadcast();
  }
}

void newConnectionCallback( bool adopt ) {
  Serial.printf("startHere: New Connection, adopt=%d\n", adopt);
  delay(5000);
  sendBroadcast();
}

void sendBroadcast() {
  if(readADC()) {
      String msg = "ON";
      //msg += mesh.getChipId();
      mesh.sendBroadcast(msg);
      Serial.println(analogRead(A0));
      Serial.println("ON");
    }
    else if(!readADC()) {
      //String msg = "ON";
      String msg = "OFF";
      //msg += mesh.getChipId();
      mesh.sendBroadcast(msg);
      Serial.println(analogRead(A0));
      Serial.println("OFF");
    }
}

