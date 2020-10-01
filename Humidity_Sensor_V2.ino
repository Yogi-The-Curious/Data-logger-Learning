/* Here I have decided to stray from the idea of input for a minute to tackle the lcd screen and then hopefully combining blinky, pwm and this program
into one slightly more robust program before moving on to tackle the data logger (and hopefully coming back to the input later) Lets Go!*/

#include <LiquidCrystal.h>
#include <dht.h>
#include <SPI.h>
#include <SD.h>
File myFile;

// initializing the lcd screen and dht sensor

const int rs = A0, en = 5, d4 = A1, d5 = A2, d6 = A3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

dht DHT;
#define DHT11_PIN 6

// Setting thresholds for blinky and pwm

float RHThreshold = 60;
float TempThreshold = 70;

int led = 9;
// Calling the Serial and LCD montiors

void setup(){
  Serial.begin(9600);
  SD.begin(10);
  pinMode (9,OUTPUT);
  lcd.begin(16,2);
  lcd.print("Initializing...");
  myFile = SD.open("test.txt", FILE_WRITE);
    if (myFile) {
  Serial.println ("Writing to test.txt");
}
  else {
    Serial.print("Something Went Wrong");
  }
  myFile.println("Temperature F, Humidity");
  
}



// chekcs temp and RH, converts C to F and then prints to serial and lcd. Trips LED.

void loop()
{ 
  for(int i=0; i < 100; i++) 
  {
   int chk = DHT.read11(DHT11_PIN);  //figure out what this means
   float temp = DHT.temperature;       //call to get temp
   float RH = DHT.humidity;            // call to get humidity 
   
  
  

   // here I manipulate the temp to farenheit
   
   temp = temp / 5.00;  
   temp = temp * 9.00;
   temp = temp + 32.00;

   /* Here we print to the serial screen, 
   Eventually this will be removed in leiu of data logging
   
   Serial.print("Rm Temp = ");
   Serial.print(temp);
   Serial.print(" F");
   Serial.println("Rm RH = ");
   Serial.print(DHT.humidity);
   Serial.println("%");*/

  //this is written without any info so that we might transpose it into excel for a graph
  
   myFile.print(temp);
   myFile.print(" ,");
   myFile.println(RH);
   

   // Here I print to the lcd screen, would be nice to somehow clean it up
   
   lcd.clear();
   lcd.print("Rm Temp = ");
   lcd.print(temp);
   lcd.print(" F");
   lcd.setCursor(0,1);
   lcd.print ("Rm RH = ");
   lcd.print (RH);
   lcd.print("%");
   
   int freq = temp;    // turns temp into a range from 0 to 255 for PWM module below
   freq = freq -32;
   freq = freq * 2.8;
   
   if (temp > TempThreshold) {
    Serial.println("We have exceeded the temp threshold");
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);                       // wait for a half second
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(500);                       // wait for a half a second
    digitalWrite(led, HIGH);  // turn on for two seconds
    delay (500);
    digitalWrite(led, LOW);
    delay(500);
   }
    else {
    analogWrite (9,freq);    //runs a PWM to LED to change brightness according to temp
   }
   Serial.println("Working \n");
 
   delay(1000);  // delays the next measurement, in line with sampling rate of DHT11
  }
  Serial.print("Done Writing");
  myFile.close();
 exit(0);  //program loops through infinite iterations otherwise
}
