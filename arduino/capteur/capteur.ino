void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  int raw_value = analogRead(A0);

  float temperature = raw_value * (5.0 / 1023.0 * 100.0);

//  Serial.print("raw-val " + raw_value + " temperature " + temperature);
  Serial.print("raw_value ");
  Serial.print(raw_value);
  Serial.print(" temperature ");
  Serial.print(temperature);
  Serial.print("\n");
  delay(2500);

}
