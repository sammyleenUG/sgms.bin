#include <ArduinoJson.h>


//BIN DETAILS
//..........................................................

int bin_id = 5;

//.......................................................

//..........................
// ULTRASONIC SENSOR
int ultrasonicPin = A1;
float distance;

//AIR QUALITY SENSOR
int airPin = A2;
float air;

//.....................................................
//WE NEED AN AVERAGE OF THE LAST TWO READINGS
const int numReadings = 2;
int readingsIndex = 0;


//HOLD LAST FIVE READINGS
float ultrasonicReadings[numReadings];
float mq135Readings[numReadings];



//PREVIOUS READINGS
//............................................................

float previousAverageDistance = 0;
float previousAveragePercentage = 0;

//................................................................


void setup() {
  Serial.begin(9600);
  
  pinMode(ultrasonicPin, INPUT);
  pinMode(airPin, INPUT);

}
void loop() {

  
    // The only messages we'll parse will be formatted in JSON
    DynamicJsonDocument doc(1024);
    
    float bin_level = getUltraReading();
    float bin_air_quality = getAirReading();
    
    readingsIndex = (readingsIndex + 1) % numReadings;
    
    // Check if the current reading is different from the previous reading
      
    if (bin_air_quality != previousAveragePercentage) {
        
      previousAveragePercentage = bin_air_quality;
      doc["air_quality"] = bin_air_quality;
        
    }else{
      doc["air_quality"] = -1.00;
    }
    
    // Check if the current reading is different from the previous reading
    if (bin_level != previousAverageDistance) {
        
      previousAverageDistance = bin_level;
    
      doc["bin_level"] = bin_level;
    }else{
      doc["bin_level"] = -1.00;
    }

    serializeJson(doc,Serial);

    delay(5000);
}


float getUltraReading()
{
  
  int ultrasonicValue = analogRead(ultrasonicPin);
  distance = (ultrasonicValue * 2) / 15;

  //  if(distance < 20){
  //    distance = distance - ((distance * 80)/100);
  //  }
  
  ultrasonicReadings[readingsIndex] = distance;
  
  float averageDistance = 0;

  //getting average
  
  for (int i = 0; i < numReadings; i++) {
    averageDistance += ultrasonicReadings[i];
  }
  
  averageDistance /= numReadings;

  //Serial.print("Average Distance: ");
  //Serial.println(averageDistance);

  return averageDistance;
  
}

float getAirReading()
{
   int airValue = analogRead(airPin);
   air = map(airValue, 0, 1023, 0, 100);
   mq135Readings[readingsIndex] = air;

   float averagePercentage = 0;
  
   //getting average
   
   for (int i = 0; i < numReadings; i++) {
      averagePercentage += mq135Readings[i];
   }
  
   averagePercentage /= numReadings;

   //Serial.print("Average Percentage: ");
   //Serial.println(averagePercentage);

   return averagePercentage;

}
