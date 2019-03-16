#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"

#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "DISABLE"

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

volatile unsigned long U18pulse;
String command;
unsigned long previousMillis = 0; // last time update
long sendInterval = 150; // interval at which to do something (milliseconds)
//unsigned long checkConnectionInterval=15000;  //it can take a while to reconnect.
//unsigned long lastHBcheckTime=0;
//unsigned long heart=0;
//unsigned long heartBeatSentTime=0;
//unsigned long heartBeatReceiveTime=0;
int led = LED_BUILTIN;
//bool sendHeartbeatNow=true;

void error(const __FlashStringHelper*err) {
  while (1);
}

void setup() {
//  Serial.setTimeout(50);
//  Serial.begin(250000);
  attachInterrupt(digitalPinToInterrupt(2), magnet_detect, RISING);//Initialize the intterrupt pin (Arduino digital pin 2)
  U18pulse = 0;

  pinMode(led, OUTPUT);
  
  delay(500);
  if ( !ble.begin(VERBOSE_MODE) )
  {error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));}
  delay(1000);
  
  // do a factory reset when needed
  //if ( ! ble.factoryReset()) { error(F("Couldn't factory reset")); }

  ble.echo(false);
  ble.info();
  ble.verbose(false);  // debug info is a little annoying after this point!
  ble.setMode(BLUEFRUIT_MODE_DATA);
   
}

void loop() {
  
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > sendInterval) {
     previousMillis = currentMillis;  
//     if (sendHeartbeatNow){
//       ble.println("*");  //send heartbeat message
//       heartBeatSentTime = millis();
//       sendHeartbeatNow=false;
//     } else {
       ble.println(U18pulse);  
//     }
//     myDelay(50);
  }

  // Echo received data
  while ( ble.available() )
  {
    int c = ble.read();
//    if (c == 42) {
//      heartBeatReceiveTime = millis();
//    } else if (c == 32) {
      U18pulse=0;
//    }
  }
  
//  //reset device if havent received a heartbeat response within the interval
//  if((millis() - lastHBcheckTime) >= checkConnectionInterval)
//  {
//    if ((heartBeatSentTime > (heartBeatReceiveTime + sendInterval + 50 ))) {
//      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
//      delay(500);                       // wait for a second
//      digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
//      //resetFunc();
//      //ble.disconnect();
//      //ble.reset();
//      //ble.begin();
//      //reset heartBeatReceiveTime
//      //sendHeartbeatNow=true;  
//      heartBeatSentTime=millis(); 
//      heartBeatReceiveTime= heartBeatSentTime+1;
//    }
//    lastHBcheckTime = millis();
//  }

//  //send the heartbeat at half the check interval
//  if(millis() - heartBeatSentTime >= 2000)
//  {
//    sendHeartbeatNow=true;
//  }
  
//  if(Serial.available()){
//    command = Serial.readStringUntil("\r");
//    if(command.equals("init\r")){
//      U18pulse=0;
//    }
//    else{
//      Serial.println(command);
//    }
//  }

}
 
void magnet_detect()//This function is called whenever a magnet/interrupt is detected by the arduino
{
   U18pulse++;
   //Serial.println(U18pulse);
}

void myDelay(int mseconds) {
  // this delay keeps the arduino working, built in delay stops most activity
  //   this type of delay seems to be needed to make the servo libarary work
  //   not sure why.
  unsigned long starttime = millis();   //going to count for a fixed time
  unsigned long endtime = starttime;
  while ((endtime - starttime) <= (unsigned long) mseconds) // do the loop
  {
    endtime = millis();                  //keep the arduino awake.
  }  
}
