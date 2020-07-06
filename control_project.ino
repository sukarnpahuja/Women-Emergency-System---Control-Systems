const double IRSENSOR=A0;                     // IR SENSOR OUTPUT TO ANALOG PIN A0
int IRSENSOR_READ;                          //  IR SENSOR VALUE READ
const int IR_THRESHHOLD=100;                  // THRESHOLD VALUE FOR IR

const double PIEZOSENSOR = A1;                    //PIEZO SENSOR OUTPUT OT ANALOG PIN A1
const int PIEZO_THRESHHOLD = 100;                 //THRESHHOLD VALUE FOR PIEZO
int PIEZO_READ;                               //PIEZO SENSOR VALUE READ

const int MOTIONSENSOR = 2;                    //MOTION SENSOR OUTPUT TO DIGITAL PIN 2
int MOTION_READ;                           //MOTION SENSOR VALUE READ

const int SWITCH = 3;                          // PUSH BUTTON OUTPUT TO DIGITAL PIN 3
int SWITCH_READ;                               // PUSH BUTTON VALUE READ


#define USE_ARDUINO_INTERRUPTS true           // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>           // Includes the PulseSensorPlayground Library.
const int Pulsewire = A2;                // PULSE RATE SENSOR OUTPUT TO ANALOG PIN A2
const int Threshold = 800;             // PULSE RATE SENSOR THRESHHOLD VALUE 
PulseSensorPlayground pulseSensor;            // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

#include <TinyGPS.h>                         // GPS LIBRARY
#include <SoftwareSerial.h>                      //GPS and GSM MODULE LIBRARY IMPORT
int state = 0;                                 //state for GSM
float gpslat, gpslon;                          //ASSIGNING VARIBALES FOR LATTITUDE AND LONGITUDE LOCATION CALCULATION

TinyGPS gps;                             // CONNECTIONS OF GPS AND GSM
SoftwareSerial sgps(4, 5);                  //(Rx,Tx) of ARDUINO [ARDUINO PIN-4(Rx)- Tx of GPS and ARDUINO PIN-5(Tx)- Rx of GPS MODULE]
SoftwareSerial sgsm(6, 7);                 //(Rx,Tx) of ARDUINO [ARDUINO PIN-6(Rx)- Tx of GSM and ARDUINO PIN-7(Tx)- Rx of GSM MODULE]

const int BUZZER = 13;                         //BUZZER OUTPUT
const int BUZZER_STATE = LOW;                  //INITIALLY SET TO LOW 

void setup() 
{                
  
   pinMode(PIEZOSENSOR, INPUT);                 // PIEZO SENSOR OUTPUT TO ARDUINO 
   pinMode(IRSENSOR,INPUT);                     //IR SENSOR OUTPUT AS INPUT TO ARDUINO
   pinMode(MOTIONSENSOR, INPUT);               // MOTION SENSOR OUTPUT AS INPUT TO ARDUINO
   pinMode(SWITCH, INPUT);                     // PUSH BUTTON OUTPUT AS INPUT TO ARDUINO
   pinMode(BUZZER, OUTPUT);                     //OUTPUT PIN TO BUZZER IS 13 
   pulseSensor.analogInput(Pulsewire);         //assign variables
   pulseSensor.setThreshold(Threshold);        // ASSIGN THRESHOLD VARIABLE TO PULSESENSOR
   sgsm.begin(9600);                           //GSM BAUD RATE
   sgps.begin(9600);                           //GPS BAUD RATE
   Serial.begin(9600);                        //PROGRAM BAUD RATE
                 
}

void loop() 
{
  IRSENSOR_READ = analogRead(IRSENSOR);      //analog input of  infrared sensor
  PIEZO_READ = analogRead(PIEZOSENSOR);      //analog input of piezo sensor
  MOTION_READ = digitalRead(MOTIONSENSOR);   //digital input of motion sensor
  SWITCH_READ = digitalRead(SWITCH);         // digital input of push button
  pulseSensor.analogInput(Pulsewire);
  int myBPM = pulseSensor.getBeatsPerMinute(); // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now.

  if(IRSENSOR_READ > IR_THRESHHOLD)          // comparing to the threshhold value set
  {
    digitalWrite(BUZZER, HIGH);               // set the BUZZER on for 1 second
    delay(1000);
    digitalWrite(BUZZER, LOW);
    delay(100);                               // set buzzer off for for milli second

    while (sgps.available())
  {
    int c = sgps.read();
    if (gps.encode(c))
    {
      gps.f_get_position(&gpslat, &gpslon);
    }
  }
    if ((IRSENSOR_READ > IR_THRESHHOLD) && state == 0) {
      sgsm.print("\r");
      delay(1000);
      sgsm.print("AT+CMGF=1\r");
      delay(1000);

      //The number the messege is to be sent to
      sgsm.print("AT+CMGS=\"+919711600080\"\r");
      delay(1000);
      
      //The text of the message to be sent.
      sgsm.print("ALERT: YOUR LOVED ONE IS IN DANGER; CLICK THE LINK BELOW TO TRACK THE LOCATION OF VERDICT:");
      sgsm.print("THE EXACT LOCATION IS:");
      sgsm.print("Latitude :");
      sgsm.println(gpslat, 6);
      sgsm.print("Longitude:");
      sgsm.println(gpslon, 6);
      sgsm.print("https://www.google.com/maps/?q=");
      sgsm.print(gpslat, 6);
      sgsm.print(",");
      sgsm.print(gpslon, 6);
      delay(1000);
      sgsm.write(0x1A);
      delay(1000);
      state = 1;
    }
  if (IRSENSOR_READ < IR_THRESHHOLD) {
      state = 0;
    }
  }

  else if(PIEZO_READ > PIEZO_THRESHHOLD)
  {
    digitalWrite(BUZZER, HIGH);               // set the BUZZER on for 1 second
    delay(1000);
    digitalWrite(BUZZER, LOW);
    delay(100);                               // set buzzer off for for milli second

     while (sgps.available())
  {
    int c = sgps.read();
    if (gps.encode(c))
    {
      gps.f_get_position(&gpslat, &gpslon);
    }
  }
    if ((PIEZO_READ > PIEZO_THRESHHOLD) && state == 0) {
      sgsm.print("\r");
      delay(1000);
      sgsm.print("AT+CMGF=1\r");
      delay(1000);

      //The number the messege is to be sent to
      sgsm.print("AT+CMGS=\"+919711600080\"\r");
      delay(1000);
      
      //The text of the message to be sent.
      sgsm.print("ALERT: YOUR LOVED ONE IS IN DANGER; CLICK THE LINK BELOW TO TRACK THE LOCATION OF VERDICT:");
      sgsm.print("THE EXACT LOCATION IS:");
      sgsm.print("Latitude :");
      sgsm.println(gpslat, 6);
      sgsm.print("Longitude:");
      sgsm.println(gpslon, 6);
      sgsm.print("https://www.google.com/maps/?q=");
      sgsm.print(gpslat, 6);
      sgsm.print(",");
      sgsm.print(gpslon, 6);
      delay(1000);
      sgsm.write(0x1A);
      delay(1000);
      state = 1;
    }
  if (PIEZO_READ < PIEZO_THRESHHOLD) {
      state = 0;
    }
  }

  else if(MOTION_READ == HIGH)
  {
    digitalWrite(BUZZER, HIGH);               // set the BUZZER on for 1 second
    delay(1000);
    digitalWrite(BUZZER, LOW);
    delay(100);                               // set buzzer off for for milli second
    Serial.print("MOTION IS DETECTED hHEREBY.");

     while (sgps.available())
  {
    int c = sgps.read();
    if (gps.encode(c))
    {
      gps.f_get_position(&gpslat, &gpslon);
    }
  }
    if ((MOTION_READ == HIGH) && state == 0) {
      sgsm.print("\r");
      delay(1000);
      sgsm.print("AT+CMGF=1\r");
      delay(1000);

      //The number the messege is to be sent to
      sgsm.print("AT+CMGS=\"+919711600080\"\r");
      delay(1000);
      
      //The text of the message to be sent.
      sgsm.print("ALERT: YOUR LOVED ONE IS IN DANGER; CLICK THE LINK BELOW TO TRACK THE LOCATION OF VERDICT:");
      sgsm.print("THE EXACT LOCATION IS:");
      sgsm.print("Latitude :");
      sgsm.println(gpslat, 6);
      sgsm.print("Longitude:");
      sgsm.println(gpslon, 6);
      sgsm.print("https://www.google.com/maps/?q=");
      sgsm.print(gpslat, 6);
      sgsm.print(",");
      sgsm.print(gpslon, 6);
      delay(1000);
      sgsm.write(0x1A);
      delay(1000);
      state = 1;
    }
  if (MOTION_READ == LOW) {
      state = 0;
    }
  }

  else if(SWITCH_READ == HIGH)
  {
    digitalWrite(BUZZER, HIGH);               // set the BUZZER on for 1 second
    delay(1000);
    digitalWrite(BUZZER, LOW);
    delay(100);                               // set buzzer off for for milli second
    
     while (sgps.available())
  {
    int c = sgps.read();
    if (gps.encode(c))
    {
      gps.f_get_position(&gpslat, &gpslon);
    }
  }
    if ((SWITCH_READ == HIGH) && state == 0) {
      sgsm.print("\r");
      delay(1000);
      sgsm.print("AT+CMGF=1\r");
      delay(1000);

      //The number the messege is to be sent to
      sgsm.print("AT+CMGS=\"+919711600080\"\r");
      delay(1000);
      
      //The text of the message to be sent.
      sgsm.print("ALERT: YOUR LOVED ONE IS IN DANGER; CLICK THE LINK BELOW TO TRACK THE LOCATION OF VERDICT:");
      sgsm.print("THE EXACT LOCATION IS:");
      sgsm.print("Latitude :");
      sgsm.println(gpslat, 6);
      sgsm.print("Longitude:");
      sgsm.println(gpslon, 6);
      sgsm.print("https://www.google.com/maps/?q=");
      sgsm.print(gpslat, 6);
      sgsm.print(",");
      sgsm.print(gpslon, 6);
      delay(1000);
      sgsm.write(0x1A);
      delay(1000);
      state = 1;
    }
  if (SWITCH_READ == HIGH) {
      state = 0;
    }
  }
  
  else if(myBPM > Threshold)                 // comaring condition of heartbeat
  {
    digitalWrite(BUZZER, HIGH);               // set the BUZZER on for 1 second
    delay(1000);
    digitalWrite(BUZZER, LOW);                
    delay(100);                               // set buzzer off for for milli second
    Serial.print("BPM: ");                   // Print phrase "BPM: "
    Serial.println(myBPM);                   // Print the value inside of myBPM.
    delay(1000);
    
    while (sgps.available())
  {
    int c = sgps.read();
    if (gps.encode(c))
    {
      gps.f_get_position(&gpslat, &gpslon);
    }
  }
    if ((myBPM > Threshold) && state == 0) {
      sgsm.print("\r");
      delay(1000);
      sgsm.print("AT+CMGF=1\r");
      delay(1000);

      //The number the messege is to be sent to
      sgsm.print("AT+CMGS=\"+919711600080\"\r");
      delay(1000);
      
      //The text of the message to be sent.
      sgsm.print("ALERT: YOUR LOVED ONE IS IN DANGER; CLICK THE LINK BELOW TO TRACK THE LOCATION OF VERDICT:");
      sgsm.print("THE EXACT LOCATION IS:");
      sgsm.print("Latitude :");
      sgsm.println(gpslat, 6);
      sgsm.print("Longitude:");
      sgsm.println(gpslon, 6);
      sgsm.print("https://www.google.com/maps/?q=");
      sgsm.print(gpslat, 6);
      sgsm.print(",");
      sgsm.print(gpslon, 6);
      delay(1000);
      sgsm.write(0x1A);
      delay(1000);
      state = 1;
    }
  if (myBPM < Threshold) {
      state = 0;
    }
  }

}
