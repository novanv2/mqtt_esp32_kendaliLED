
const uint8_t SHARP_LED_PIN = 13;   // Sharp Dust/particle sensor Led Pin
const uint8_t SHARP_VO_PIN = 34;    // Sharp Dust/particle analog out pin used for reading 

GP2YDustSensor dustSensor(GP2YDustSensorType::GP2Y1010AU0F, SHARP_LED_PIN, SHARP_VO_PIN);

void defDust() {
  //Serial.begin(115200);
  //dustSensor.setBaseline(0.4); // set no dust voltage according to your own experiments
  //dustSensor.setCalibrationFactor(1.1); // calibrate against precision instrument
  dustSensor.begin();
}

void bacaDust() {
  int dustDens = dustSensor.getDustDensity();
  int runAve = dustSensor.getRunningAverage();
  Serial.print("Dust density: ");
  Serial.print(dustDens);
  Serial.print(" ug/m3; Running average: ");
  Serial.print(runAve);
  Serial.println(" ug/m3");
  //delay(1000);
  
}
