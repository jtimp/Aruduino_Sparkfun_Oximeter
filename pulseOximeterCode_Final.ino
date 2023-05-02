/*
 This example sketch gives you exactly what the SparkFun Pulse Oximiter and
 Heart Rate Monitor is designed to do: read heart rate and blood oxygen levels.
 This board requires I-squared-C connections but also connections to the reset
 and mfio pins. When using the device keep LIGHT and CONSISTENT pressure on the
 sensor. Otherwise you may crush the capillaries in your finger which results
 in bad or no results. This differs from example one by giving an additional
 two data points: an extended finger status and the r value of the blood oxygen
 level. A summary of the hardware connections are as follows: 
 SDA -> SDA
 SCL -> SCL
 RESET -> PIN 4
 MFIO -> PIN 5

 Author: Elias Santistevan
 Date: 8/2019
 SparkFun Electronics

 If you run into an error code check the following table to help diagnose your
 problem: 
 1 = Unavailable Command
 2 = Unavailable Function
 3 = Data Format Error
 4 = Input Value Error
 5 = Try Again
 255 = Error Unknown
*/

#include <SparkFun_Bio_Sensor_Hub_Library.h>
#include <Wire.h>
#include <LiquidCrystal.h>    // This tells the arduino to use code from the LiquidCrystal Library.

// Reset pin, MFIO pin
int resPin = 10;
int mfioPin = 9;

// LEDs
int redLED = 12;
int blueLED = 13;

// Takes address, reset pin, and MFIO pin.
SparkFun_Bio_Sensor_Hub bioHub(resPin, mfioPin); 

bioData body;  
// ^^^^^^^^^
// What's this!? This is a type (like int, byte, long) unique to the SparkFun
// Pulse Oximeter and Heart Rate Monitor. Unlike those other types it holds
// specific information on your heartrate and blood oxygen levels. BioData is
// actually a specific kind of type, known as a "struct". 
// You can choose another variable name other than "body", like "blood", or
// "readings", but I chose "body". Using this "body" varible in the 
// following way gives us access to the following data: 
// body.heartrate  - Heartrate
// body.confidence - Confidence in the heartrate value
// body.oxygen     - Blood oxygen level
// body.status     - Has a finger been sensed?
// body.extStatus  - What else is the finger up to?
// body.rValue     - Blood oxygen correlation coefficient.  

LiquidCrystal lcd(2,3,4,5,6,7,8);  // Sets up the LCD for use
                                      // Gives the code LiquidCrystal id(rs,enable,d4,d5,d6,d7)                                                    
                                      // The id in this case is lcd—this lets the arduino know what display is being used.
                                      // Numbers represent the ports that the wires go to.
                                      // rs is the orange wire going to port 2 on the arduino.
                                      // Read/Write is the yellow wire going to port 3.
                                      // enable is the green wire (port 4)
                                      // d4 is the red wire (port 5)
                                      // d5 is the brown wire (port 6)
                                      // d6 is the black wire (port 7)
                                      // d7 is the purple wire (port 8)


void setup(){
  pinMode(blueLED, OUTPUT); 
  pinMode(redLED, OUTPUT); 
  digitalWrite(redLED, HIGH);
  Serial.begin(115200);

  Wire.begin();
  int result = bioHub.begin();
  if (result == 0) //Zero errors!
    Serial.println("Sensor started!");
  else
    Serial.println("Could not communicate with the sensor!");
 
  Serial.println("Configuring Sensor...."); 
  int error = bioHub.configBpm(MODE_TWO); // Configuring just the BPM settings. 
  if(error == 0){ // Zero errors
    Serial.println("Sensor configured.");
  }
  else {
    Serial.println("Error configuring sensor.");
    Serial.print("Error: "); 
    Serial.println(error); 
  }

  // Data lags a bit behind the sensor, if you're finger is on the sensor when
  // it's being configured this delay will give some time for the data to catch
  // up. 
  Serial.println("Loading up the buffer with data....");
  delay(4000); 
  digitalWrite(redLED, LOW);
  digitalWrite(blueLED, HIGH);

  lcd.begin(20,4);               // name.begin(cols,rows) is the code that gives the dimensions of the LCD (20 columns, 4 rows)
  lcd.clear();                   // Has the LCD blank the screen
  lcd.setCursor(0,0);            // sets the cursor to column 0, row 0  (all numbers start at 0, not 1)
  lcd.print("Heart Rate: ");     // name.print(something) is the code  to display something on the LCD. 
  lcd.setCursor(0,1);            // set cursor to column 0, row 2
  lcd.print("Oxygen Level: "); 
  lcd.setCursor(0,2);            // set cursor to column 0, row 3
  lcd.print("Finger Status: ");
  lcd.setCursor(0,3);            // set cursor to column 0, row 4
  lcd.print("Sensor Status: ");
}

void loop()
{
    // Information from the readBpm function will be saved to our "body"
    // variable.  
    body = bioHub.readBpm();
    Serial.print("Heartrate: ");
    Serial.println(body.heartRate); 
    Serial.print("Confidence: ");
    Serial.println(body.confidence); 
    Serial.print("Oxygen: ");
    Serial.println(body.oxygen); 
    Serial.print("Status: ");
    Serial.println(body.status); 
    Serial.print("Extended Status: ");
    Serial.println(body.extStatus); 
    Serial.print("Blood Oxygen R value: ");
    Serial.println(body.rValue); 
    Serial.println("================================================");


    lcd.setCursor(12,0);                             // this sets the starting point at the forth column and the first row 
    lcd.print(String(body.heartRate) + "bpm "); 
    lcd.setCursor(14,1);
    lcd.print(String(body.oxygen) + "%O2 ");
    lcd.setCursor(15,2);
    lcd.print(String(body.status));
    lcd.setCursor(15,3);
    lcd.print(String(body.extStatus));

    
    // Slow it down or your heart rate will go up trying to keep up
    // with the flow of numbers
    delay(5000); 
}
