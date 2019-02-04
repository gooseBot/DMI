volatile byte U18pulse;
String command;
unsigned long previousMillis = 0; // last time update
long interval = 150; // interval at which to do something (milliseconds)

void setup() {
   Serial.setTimeout(50);
   Serial.begin(500000);
   attachInterrupt(0, magnet_detect, RISING);//Initialize the intterrupt pin (Arduino digital pin 2)
   U18pulse = 0;
}

void loop() {
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval) {
     previousMillis = currentMillis;  
     Serial.println(U18pulse);
  }
  
  if(Serial.available()){
    command = Serial.readStringUntil("\r");
    if(command.equals("init\r")){
      U18pulse=0;
    }
    else{
      Serial.println(command);
    }
  }
  
}
 
void magnet_detect()//This function is called whenever a magnet/interrupt is detected by the arduino
{
   U18pulse++;
   //Serial.println(U18pulse);
}
