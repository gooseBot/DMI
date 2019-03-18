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
int led = LED_BUILTIN;
boolean countHold=true;

void error(const __FlashStringHelper*err) {
  while (1);
}

void setup() {
//  Serial.setTimeout(50);
//  Serial.begin(250000);
  attachInterrupt(digitalPinToInterrupt(3), magnet_detect, RISING);//Initialize the intterrupt pin (Arduino digital pin 2)
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
  //if((millis() - previousMillis > sendInterval) && !countHold) {
  if((millis() - previousMillis > sendInterval)) {
    // Seems this works best to send string in one packet.
    previousMillis = millis();
    ble.print("{");  
    ble.print(U18pulse);
    ble.print("}");  
    myDelay(10);
  }

  // Echo received data
  while ( ble.available() )
  {
    int c = ble.read();
    switch (c) {
      case 99:
        U18pulse=0;
        break;
      case 114:
        countHold=false;
        break;
      case 115:
        countHold=true;
        break;
      default:
        break;
    }
  }
  
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
   if (!countHold) {
    U18pulse++;
    //Serial.println(U18pulse);
   }
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
