// define pin
const int trigPin = 9;
const int echoPin = 10;
const int vaultPin = 5;


// define variable
long duration;
int distance;
int OnDistance;

// Define input and output
void setup(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(vaultPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds, this will create pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance= duration*0.034/2;
  
OnDistance = distance;
  
  // check if cup in a suitable distance to fill
  if (OnDistance <= 10){
      delay(2000);
      digitalWrite(vaultPin, HIGH);
      Serial.println("ON");
      Serial.println("filling the cup");
      delay(5000);
      
      // while loop to check if the cup has been removed or not
        while(OnDistance <= 10){
            digitalWrite(vaultPin, LOW);
            Serial.println("Please remove the cup");
          
            // Clears the trigPin
            digitalWrite(trigPin, LOW);
            delayMicroseconds(2);

            // Sets the trigPin on HIGH state for 10 micro seconds, this will create pulse
            digitalWrite(trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);
        }
    }    
    else{
      digitalWrite(vaultPin, LOW);
      Serial.println("OFF");
    }
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
}
