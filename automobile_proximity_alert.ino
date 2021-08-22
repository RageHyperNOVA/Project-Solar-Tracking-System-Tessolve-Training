// Proximity Alert System - Automobile Driving Assistance
void setup() {
 Serial.begin(9600); // User can be given the data via infotainment system
 pinMode(2, INPUT); // Ultrasonic Sensor 1 placed in front of vehicle
 pinMode(6, INPUT); // Ultrasonic Sensor 2 placed at the back of vehicle
 pinMode(29, OUTPUT); // RED LED PLACED INSIDE THE VEHICLE FOR CLOSE CONTACT WITH VEHICLES ON ROAD
}

int uson_value1;
int uson_value2;
int threshold  = 25; // Alert system is activated if the distance btw user' vehicle and others is less than threshold value
void loop() {
  uson_value1 = analogRead(2);
  uson_value2 = analogRead(6);

  if(uson_value1 < threshold) {
    digitalWrite(29, HIGH);
    Serial.print("OBJECT/VEHICLE DETECTED IN FRONT OF YOUR VEHICLE WITHIN A DISTANCE OF");
    Serial.print(uson_value1);
    Serial.println("");
  }
  if(uson_value2 < threshold) {
    digitalWrite(29, HIGH);
    Serial.println("OBJECT/VEHICLE DETECTED AT THE BACK OF YOUR VEHICLE WITHIN A DISTANCE OF");
    Serial.print(uson_value2);
    Serial.println("");
  }

  delay(1000);
}
