// define pin
const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 13;


// define variable
long duration;
int distance;
int OnDistance;

void setup(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance= duration*0.034/2;
  
OnDistance = distance;
  if (OnDistance <= 7){
      delay(2000);
      digitalWrite(ledPin, HIGH);
      Serial.println("ON");
      Serial.println("filling the cup");
      delay(5000);
      
      // while loop to check if the cup has been removed or not
        while(OnDistance <= 7){
            digitalWrite(ledPin, LOW);
            Serial.println("Please remove the cup");
             // Clears the trigPin
            digitalWrite(trigPin, LOW);
            delayMicroseconds(2);
            duration = pulseIn(echoPin, HIGH);
                
            // Sets the trigPin on HIGH state for 10 micro seconds
            digitalWrite(trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);
            distance= duration*0.034/2;
            OnDistance = distance;
        }
    }    
    else{
      digitalWrite(ledPin, LOW);
      Serial.println("OFF");
    }
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
}
