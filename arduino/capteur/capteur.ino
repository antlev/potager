/*
    Alexandre GARCIA - Antoine LEVY - Clémentine Thornary
    garden-project - arduino_src
*/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  int raw_value = analogRead(A0);

  float temperature = raw_value * (5.0 / 1023.0 * 100.0);

//  Serial.print("raw-val " + raw_value + " temperature " + temperature);
  Serial.print("# ");
  Serial.print(temperature);
  Serial.print(" *");
  delay(2500);

}
