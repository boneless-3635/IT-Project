int vaultPin = 4; //pin for solenoid , can be differ
void setup() {
// setup code for pin
pinMode(vaultPin, OUTPUT); //Sets the pin as an output
}
void(* resetFunc) (void) = 0; // reset function 
void loop() {
// running the machine code
digitalWrite(vaultPin, HIGH); //Switch Solenoid ON
delay(1000); //Change this to the control the amount of liquid
digitalWrite(vaultPin, LOW); //Switch Solenoid OFF
resetFunc();// reset adruino
}
