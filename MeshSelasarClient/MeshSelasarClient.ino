#include <easyMesh.h>
//#include <SimpleList.h>
//#include <ArduinoJson.h>
//ID: xxxxx327
//ID: tes hang xxxxxxx799

//sebelum menerima perintah, assign role dulu
//baca eeprom, apakah sudah ada nomor hierarki atau tidak, bila ya langsung kerja, bila tidak maka lanjut ke step 3
//scan daerah sekitar, bila ada server maka lgsg hierarki jadi bernomor 2
//bila tidak ada maka rank hierarki jadi meminta ke node sekitar, dan node sekitar semuanya akan menjawab dengan angka hierarki (n+1)
//si node ini akan mengambil hierarki tertinggi yang bisa dia ambil
//begitu sudah dapat nomor hierarki catat di eeprom, dan lanjutkan kerja seperti biasa
//tidak akan mau menerima 'Perintah' dari node dengan hierarki di bawah dia


#define MESH_PREFIX "LampuSelasarFisika"
#define MESH_PASSWORD "labiot2016"
#define MESH_PORT 5555
#define pinReset1 13 
#define pinReset2 14
#define pinSet1 12
#define pinSet2  16
#define pinLED 5
bool LED_ON = true;

easyMesh mesh;

//deteksi LDR, bila dibawah nilai tertentu broadcast perintah turn on

const float noon = 1.0; //ambil data dulu
unsigned long last = millis()+900000;   //agar saat menyala langsung membaca
unsigned long now = millis();
unsigned long updateTime = 900000;  //15 menit
bool OFF=false;
bool ON = false;
bool ExpMode = false;

unsigned long last_LED = millis();
unsigned long update_LED = 1000;


void setup() {
  Serial.begin(115200);
  mesh.setDebugMsgTypes(ERROR|STARTUP);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, MESH_PORT);
  mesh.setReceiveCallback(&receivedCallback);
  mesh.setNewConnectionCallback( &newConnectionCallback );
  pinMode(pinReset1,OUTPUT); //TURN OFF
  pinMode(pinReset2,OUTPUT);
  pinMode(pinSet1,OUTPUT);  //TURN ON
  pinMode(pinSet2,OUTPUT);
  //pinMode(pinLED,OUTPUT);
  digitalWrite(pinSet1, LOW);
  digitalWrite(pinSet2, LOW);
  digitalWrite(pinReset1, LOW);
  digitalWrite(pinReset2, LOW); 
}

void loop() {  
  mesh.update();
  if(OFF) turnOff();
  else if(ON) turnOn();
  if(millis()-last_LED>update_LED) {
    Serial.print("SEND");
    broadcast();
    last_LED = millis();
    //digitalWrite(pinLED, LED_ON);
    //LED_ON = !LED_ON;
  }
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %d msg=%s\n", from, msg.c_str());
  broadcast2();
  if(msg=="ON") {
    //turn ON
    Serial.println("Turn it ON");
    ON = true;
    //turnOn();
    broadcast();
  }
  else if(msg=="OFF") {
    Serial.println("Turn it off");
    //turnOff();
    OFF = true;
    broadcast();
  }
  else if(msg=="ExpModeON") {
    ExpMode = true;
  }
  else if(msg=="ExpModeOFF") {
    ExpMode = false;
  }
  else{
    //expect angka tanda upload
    
  }
}

void newConnectionCallback( bool adopt ) {
  Serial.printf("startHere: New Connection, adopt=%d\n", adopt);
  //delay(1000);
  if(ExpMode) {
    
  }
}

void broadcast() {
  String msg = "ack";
  mesh.sendBroadcast(msg);
}


void broadcast2() {
  String msg = "received";
  mesh.sendBroadcast(msg);
}

void turnOff() {
    digitalWrite(pinReset1, HIGH);
    digitalWrite(pinReset2, HIGH);
    delay(100);
    digitalWrite(pinReset1, LOW);
    digitalWrite(pinReset2, LOW); 
    OFF = false;
}

void turnOn() {
    digitalWrite(pinSet1, HIGH);
    digitalWrite(pinSet2, HIGH);
    delay(100);
    digitalWrite(pinSet1, LOW);
    digitalWrite(pinSet2, LOW);
    ON = false;
}

