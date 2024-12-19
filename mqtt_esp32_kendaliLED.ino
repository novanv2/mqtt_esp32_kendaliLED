#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h>               
#include <TimeLib.h>
#include <GP2YDustSensor.h>
#include "motor.h"
#include "servoset.h"
#include "setntp.h"
#include "setupwifi.h"
#include "dustsen.h"

//------Variable Relay-------
#define relay1 27
#define relay2 22
#define pb 18

//const char* ssid = "Iyd";
//const char* password = "50acnar20";

const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
char msg1[MSG_BUFFER_SIZE];
char msg2[MSG_BUFFER_SIZE];
char msg3[MSG_BUFFER_SIZE];

int jam;
int menit;
int jam2;
int menit2_;

unsigned long lastPesan = 0;  

bool manualMode = true;
bool autoMode1 = false;
bool autoMode2 = false;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String payloadString;
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    payloadString += (char)payload[i];
    Serial.print(payloadString);
  }
  Serial.println();

  if ((strcmp(topic,"apaa2") == 0) && (manualMode == true)){   //Menyalakan Motor
    if ((char)payload[0] == '1') {
    motorkanan();
  }
    else if ((char)payload[0] == '2'){
    motorkiri();
  }
    else{
    motormati();
  }
 }
  if ((strcmp(topic,"apaa3") == 0) && (manualMode == true)){    //Menyalakan RELAY
    if ((char)payload[0] == '1'){
        digitalWrite(relay1, LOW); 
    }
    else if((char)payload[0] == '0'){
        digitalWrite(relay1, HIGH);
    }
    else if((char)payload[0] == '2'){
        digitalWrite(relay2, LOW);
    }
    else if((char)payload[0] == '3'){
        digitalWrite(relay2, HIGH);
    }
  }
  
  if (strcmp(topic,"apaa4") == 0){      //Pemilihan MODE
    if ((char)payload[0] == '2'){
       manualMode = false;
       autoMode1 = true;
       autoMode2 = false;
    }
    else if ((char)payload[0] == '1'){
       manualMode = true;
       autoMode1 = false;
       autoMode2 = false;
    }
    else if ((char)payload[0] == '3'){
       manualMode = false;
       autoMode1 = false;
       autoMode2 = true;
    }
  }
  
  if ((strcmp(topic,"apaa5") == 0) && (manualMode == false)){       //Input Jam dan Menit Mode Jadwal 1
     jam = payloadString.toInt();
  }
  if ((strcmp(topic,"apaa6") == 0) && (manualMode == false)){
     menit = payloadString.toInt();
  }
  if ((strcmp(topic,"apaa7") == 0) && (manualMode == false)){       //Input Jam dan Menit Mode Jadwal 2
     jam2 = payloadString.toInt();
  }
  if ((strcmp(topic,"apaa8") == 0) && (manualMode == false)){
     menit2_ = payloadString.toInt();
  }


  if ((strcmp(topic,"apaa9") == 0) && (manualMode == true)){
    if ((char)payload[0] == '1'){
        //servo90();
    }
    else if ((char)payload[0] == '0'){
        //servo0();
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("apaa1", "halo bang");
      client.publish("apaa15", "halo bang");
      client.publish("apaa14", "halo bang");
      // ... and resubscribe
      client.subscribe("apaa2");
      client.subscribe("apaa3");
      client.subscribe("apaa4");
      client.subscribe("apaa5");
      client.subscribe("apaa6");
      client.subscribe("apaa7");
      client.subscribe("apaa8");
      client.subscribe("apaa9");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/*
void modeOto1(){
  if ((hour_ == jam) && (minute_ == menit) && (second_ >= 0) && (second_ <= 20)) //(minute_ <= 1))
  {
    digitalWrite(relay1, LOW);
    motorkanan();
  }
  else if ((hour_ == jam) && (minute_ == menit) && (second_ >= 23) && (second_ <= 43)) //(minute_ <= 1))
  {
    digitalWrite(relay1, LOW);
    motorkiri();
  }
  else if ((hour_ == jam) && (minute_ == menit+1) )
  {
    digitalWrite(relay2, LOW);
  }
  else if ((hour_ == jam) && (minute_ == menit+2) && (second_ >= 1) && (second_ <= 21)) //(minute_ <= 1))
  {
    motorkanan();
  }
  else if ((hour_ == jam) && (minute_ == menit+2) && (second_ >= 24) && (second_ <= 44)) //(minute_ <= 1)) 
  {
    motorkiri();
  }
  else{
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    motormati();
  }
}
*/

void modeOto1(){
  if ((hour_ == jam) && (minute_ >= menit) && (minute_ <= menit + 5)) //(minute_ <= 1))
  {
    //nyalaAlat();
    nyala_alat2();
  }
  else if ((hour_ == jam2) && (minute_ >= menit2_) && (minute_ <= menit2_ + 5)) //(minute_ <= 1))
  {
    nyala_alat2();
  }  
  else{
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    motormati();
  }
}

void modeOto2(){
  ///*
  int dustDens2 = dustSensor.getDustDensity();
  int runAve2 = dustSensor.getRunningAverage();

  if ((dustDens2 >= 100) && (dustDens2 <= 150)){
    nyalaAlat();
  }
  else{
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    motormati();
  }
  //*/
  //nyalaAlat();
}

const unsigned long relay1OnTime = 85000;  // Waktu relay1 menyala (20 detik)
const unsigned long relay1OnTime2 = 83000;
const unsigned long relay1OffTime = 2000;  // Waktu relay1 mati (2 detik)
const unsigned long relay2OnTime = 20000;  // Waktu relay2 menyala (20 detik)
unsigned long currentMillis;  // Waktu sekarang
unsigned long previousMillis;  // Waktu sebelumnya
unsigned long interval;  // Interval waktu yang sedang berlangsung
int step = 1;  // Langkah saat ini

void nyalaAlat(){
    // Mendapatkan waktu sekarang
  currentMillis = millis();

  // Periksa apakah interval waktu telah berlalu
  if (currentMillis - previousMillis >= interval) {
    // Mengupdate waktu sebelumnya
    previousMillis = currentMillis;

    // Memeriksa langkah saat ini
    switch (step) {
      case 1:
        //alat berjalan kiri
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, LOW);
        motorkanan();
        //interval = relay1OnTime;
        step = 2;
        break;
      case 2:
        // alat mati 2 detik
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, HIGH);
        motormati();
        interval = relay1OffTime;
        step = 3;
        break;
      case 3:
        // alat berjalan kanan
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, HIGH);
        motorkiri();
        //interval = relay1OnTime2;
        step = 4;
        break;
      case 4:
        // alat berjalan kanan
        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, HIGH);
        motormati();
        interval = relay1OffTime;
        step = 5;
        break;
      case 5:
        // alat berjalan kanan
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, LOW);
        motorkanan();
        //interval = relay1OnTime;
        step = 6;
        break;
      case 6:
        // alat berjalan kanan
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, HIGH);
        motormati();
        interval = relay1OffTime;
        step = 7;
        break;
      case 7:
        // alat berjalan kanan
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, HIGH);
        motorkiri();
        //interval = relay1OnTime2;
        step = 8;
        break;
/*
      case 4:
        // Langkah 3: relay1 mati, relay2 menyala selama 20 detik
        digitalWrite(relay1, HIGH);
        motormati();
        digitalWrite(relay2, LOW);
        interval = relay2OnTime;
        step = 5;
        break;
      case 5:
        // Langkah 4: relay1 menyala lagi selama 20 detik
        digitalWrite(relay1, HIGH);
        motorkanan();
        digitalWrite(relay2, HIGH);
        interval = relay1OnTime;
        step = 6;
        break;
      case 6:
        // Langkah 5: relay1 mati selama 2 detik
        digitalWrite(relay1, HIGH);
        motormati();
        interval = relay1OffTime;
        step = 7;
        break;
      case 7:
        // Langkah 6: relay1 menyala lagi selama 20 detik
        digitalWrite(relay1, HIGH);
        motorkiri();
        interval = relay1OnTime;
        step = 8;
        break;
      case 8:
        // Langkah 7: relay1 mati selama 2 detik
        digitalWrite(relay1, HIGH);
        motormati();
        interval = relay1OffTime;
        step = 9;
        break;
        */
      case 8:
        // Langkah 8: relay1 dan relay2 mati
        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, HIGH);
        motormati();
        // Kembali ke langkah 1
        //interval = relay1OnTime;
        step = 1;
        break;
    }
  }
}

int alatStatus;
int alatStatus2;
int arah = 0;
int countArah1 = 0;
int countArah2 = 0;

void nyala_alat2(){
 if (arah == 0 && countArah1 < 2){
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  motorkanan();
 }
 else if(arah == 1 && countArah2 < 2){
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, HIGH);
  motorkiri();
 }
 else if(countArah2 == 2){
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  motormati();
 }
  if (buttonState == LOW && lastButtonState == HIGH) {
    // Push button ditekan, ubah status alat
    if (alatStatus == 0) {
      arah = 1;
      alatStatus = 1;  // Mengubah status ke ON
      countArah1++;
    } 
    /*
    else {
      motorkanan();
      alatStatus = 0;  // Mengubah status ke OFF
    }
    */
  }
  if (buttonState2 == LOW && lastButtonState2 == HIGH) {
    // Push button ditekan, ubah status alat
    if (alatStatus == 1) {
      arah = 0;
      alatStatus = 0;  // Mengubah status ke ON
      countArah2++;
    } 
    /*
    else {
      motorkiri();
      alatStatus2 = 0;  // Mengubah status ke OFF
    }
    */
  }   
}


//----------------------------SETUP------------------------------------
//SETUP
void setup() {
  Serial.begin(115200);
  defDust();
  //connectToWiFi();

  //setup servo
  setupPB();

  //setup relay
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  
  wifiRecon();
  //setup_wifi();
 // Serial.print("RSSI: ");
  //Serial.println(WiFi.RSSI());
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  inputmotor();
 // dustSensor.begin();

 //----------MULAI NTP------------
  timeClient.begin();
  //timeClient.update();
}

//-----------------------------LOOP-----------------------------------
//LOOP

void loop() {
  timeClient.update();
  waktuntc();

  bacaPB();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if(!manualMode && !autoMode2){
    modeOto1();
  }
  if(!manualMode && !autoMode1){
    modeOto2();
  }
  
  //client.setCallback(callback);

  unsigned long now = millis();
  if (now - lastMsg >= 1000){
    lastMsg = now;
    bacaDust();    
      Serial.print(jam);
      Serial.print(menit);
      Serial.println();
  }

  int dustDens = dustSensor.getDustDensity();
  int runAve = dustSensor.getRunningAverage();
  
  char dustSensorChar[10];
  char dustAveChar[10];

  sprintf(dustSensorChar, "%d", dustDens);
  sprintf(dustAveChar, "%d", runAve);

  unsigned long tnow = millis();
  if (tnow - lastPesan >= 1000) {
    lastPesan = tnow;
    snprintf (msg, MSG_BUFFER_SIZE, Time);
    snprintf (msg1, MSG_BUFFER_SIZE, Date);
    snprintf (msg2, MSG_BUFFER_SIZE, dustSensorChar);
    snprintf (msg3, MSG_BUFFER_SIZE, dustAveChar);
    Serial.print("Publish message: ");
    Serial.print(msg1);
    Serial.println(msg);
    client.publish("apaa1", msg);
    client.publish("apaa15", msg1);
    client.publish("apaa14", msg2);
    client.publish("apaa13", msg3);
  }
}
