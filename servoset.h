// Sertakan library untuk Servo
//#include <Servo.h>

// Deklarasikan objek Servo dan variabel
//Servo myservo;

// Deklarasikan konstanta untuk pin-pin
//const byte servoPin = 18;
#define pb 18
#define pb2 19

int buttonState;
int buttonState2;
int lastButtonState;
int lastButtonState2;

// Fungsi setup untuk dijalankan sekali pada awal program
void setupPB() {
  // Hubungkan objek servo ke pin yang ditentukan
  pinMode(pb, INPUT_PULLUP);
  pinMode(pb2, INPUT_PULLUP);

  buttonState = digitalRead(pb);
  buttonState2 = digitalRead(pb2);
}

void bacaPB(){
  lastButtonState = buttonState;
  buttonState = digitalRead(pb);

  //Serial.println(buttonState);
 // Serial.print("last1 = ");
 // Serial.println(lastButtonState);

  lastButtonState2 = buttonState2;
  buttonState2 = digitalRead(pb2);

  //Serial.println(buttonState2);
 // Serial.print("last2 = ");
 // Serial.println(lastButtonState2);
}

//void servo90(){
  //myservo.write(90);
//}

//void servo0(){
  //myservo.write(0);
//}
