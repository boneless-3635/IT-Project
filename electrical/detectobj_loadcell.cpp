
#include <HX711_ADC.h>

// define pin ( using const to make sure the value will not chance during the process)
const int vaultPin = 4;
const int fill_ledPin = 6;
const int remove_ledPin = 7;
const int HX711_dout = 2; //mcu > HX711 dout pin
const int HX711_sck = 3; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);
unsigned long t = 0;


// define variable
float cup_weight;
// Define input and output
void setup(){
  pinMode(vaultPin, OUTPUT);
  pinMode(fill_ledPin, OUTPUT);
  pinMode(remove_ledPin, OUTPUT);
  Serial.begin(9600);
  delay(10);
  
  //Booting up the machine
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
  if ((cup_weight >= 4) && (cup_weight <=300 )){   //change the cup_weight on the right to increase the desired weight
      delay(3000);
      digitalWrite(vaultPin, HIGH);
      digitalWrite(fill_ledPin, HIGH);
      Serial.println("ON");
      Serial.println("filling the cup");
    
      // update the cup_weight constantly in order to stop at the exact weight
      while(cup_weight >= 4 && cup_weight <= 300){
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
      }
      digitalWrite(fill_ledPin, LOW);  
      
     // while loop to check if the cup has been removed or not
       while(cup_weight >= 10 && cup_weight <= 1000){
         digitalWrite(vaultPin, LOW);
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
            digitalWrite(remove_ledPin, HIGH);
            Serial.println("Please remove the cup");
        }  
    }
      else{
      digitalWrite(remove_ledPin, LOW);
      digitalWrite(vaultPin, LOW);
      Serial.println("OFF");
    }
}  
