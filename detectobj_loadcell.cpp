#include <HX711_ADC.h>

// define pin ( using const to make sure the value will not chance during the process)
const int vaultPin = 3;
const int pumpPin = 6;
const int fill_ledPin = 2;
const int remove_ledPin = 7;
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 5; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);
unsigned long t = 0;


// define variable
float cup_weight;

// Define input and output
void setup(){
  pinMode(vaultPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(fill_ledPin, OUTPUT);
  pinMode(remove_ledPin, OUTPUT);
  Serial.begin(9600);
  delay(10);
  Serial.println();
  Serial.println("Starting...");
  LoadCell.begin();
  float calibrationValue; // calibration value (see example file "Calibration.ino")
  calibrationValue = 952.77 ;
  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }
}


void loop() {
  //weighting
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

   //check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

   //get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      cup_weight = LoadCell.getData();
      Serial.print("Load_cell output val: ");
      Serial.println(cup_weight);
      newDataReady = 0;
      t = millis();
    }
  }
  // check if cup in a suitable weight to fill
  if ((cup_weight >= 4) && (cup_weight <=80 )){
      delay(2000);
      digitalWrite(vaultPin, HIGH);
      digitalWrite(pumpPin, HIGH);
      digitalWrite(fill_ledPin, HIGH);
      Serial.println("ON");
      Serial.println("filling the cup");
      delay(5000);
      digitalWrite(fill_ledPin, LOW);  
      
      // while loop to check if the cup has been removed or not
        while(cup_weight >= 4 && cup_weight <= 80){
            digitalWrite(vaultPin, LOW);
            digitalWrite(pumpPin, LOW);
            digitalWrite(remove_ledPin, HIGH);
            
              //weighting
            static boolean newDataReady = 0;
            const int serialPrintInterval = 0; //increase value to slow down serial print activity

             //check for new data/start next conversion:
            if (LoadCell.update()) newDataReady = true;

             //get smoothed value from the dataset:
            if (newDataReady) {
              if (millis() > t + serialPrintInterval) {
                cup_weight = LoadCell.getData();
                Serial.print("Load_cell output val: ");
                Serial.println(cup_weight);
                newDataReady = 0;
               t = millis();
              }
            } 
            Serial.println("Please remove the cup");
        }  
    
  }
      else{
      digitalWrite(remove_ledPin, LOW);
      digitalWrite(vaultPin, LOW);
      digitalWrite(pumpPin, LOW);
     Serial.println("OFF");
    }
}
