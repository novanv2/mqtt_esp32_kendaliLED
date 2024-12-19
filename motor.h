int IN1 = 16; // deklarasi pin IN1
int IN2 = 17;  // deklarasi pin IN2
int IN3 = 25; // deklarasi pin IN3
int IN4 = 26;  // deklarasi pin IN4
int ENA = 32;
int ENB = 33;

void motorkanan(){
  // Motor A dan B berputar ke kanan 2000 ms (2 detik) dengan kecepatan penuh
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 190); // Mengatur kecepatan motor A (0-255)
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 255); // Mengatur kecepatan motor B (0-255)
}

void motorkiri(){
  // Motor A dan B berputar ke kiri 2000 ms (2 detik) dengan kecepatan penuh
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 190); // Mengatur kecepatan motor A (255 = 100%)
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 255); // Mengatur kecepatan motor B (255 = 100%)
}

void motormati(){
  // Motor A dan motor B berhenti selama 2000 ms (2 detik)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}

void inputmotor(){
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
}
