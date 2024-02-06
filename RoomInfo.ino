#include <SimpleDHT.h>
#include <LiquidCrystal_I2C.h>
#include <RtcDS1302.h>
#include <ThreeWire.h>


int cali_fac_temp= -5 ; //Value in degrees (Calibrated for Belgrage) (Humidity NOT CALIBRATED!)
// for DHT11, 
//      VCC: 5V 
//      GND: GND
//      DATA: 2 (Digital)
// for LCD 
//      VCC: 5V
//      GND: GND 
//      SDA: A4 (Analog)
//      SCL: A5 (Analog)
int pinDHT11 = 2; // Pin alocation 

SimpleDHT11 dht11(pinDHT11); //Pin Alocation 
LiquidCrystal_I2C lcd(0x27, 16, 2); //LCD alocation 
ThreeWire myWire(7,6,8); // DAT,CLK,RST 
RtcDS1302<ThreeWire>Rtc(myWire); // Groping the wires 


void setup() {
  Serial.begin(9200); 
  //Initializing Real Time Clock
  Rtc.Begin(); 
  
  RtcDateTime currentTime=RtcDateTime(__DATE__,__TIME__); 
  Rtc.SetDateTime(currentTime); 
}

void loop() {
//Activating Real Time clock 
  RtcDateTime now = Rtc.GetDateTime(); 
   // DTH read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }
  
  temperature=temperature+cali_fac_temp; //Calibration factor included 

  //Initializing LCD 
  lcd.begin();
  
  int dow = now.DayOfWeek();
  
  lcd.backlight();
  lcd.setCursor(0,0); 
  lcd.print("Temp:"); lcd.print((int)temperature);lcd.print ("  "); lcd.print ("Humi:"); lcd.print((int)humidity); 
  lcd.setCursor(1,1); 
  lcd.print(now.Hour()); lcd.print (":"); lcd.print(now.Minute()); lcd.print(" "); lcd.print (now.Day()); lcd.print("."); lcd.print (now.Month());lcd.print (" "); Serial.println(now.Hour());
  
  //Printing out Day of Week using switch case 
  switch(dow){
     case 1:
     lcd.print("Mon");
     break;
     case 2:
     lcd.print("Tue");
     break;
     case 3:
     lcd.print("Wed");
     break;
     case 4:
     lcd.print("Thu");
     break;
     case 5:
     lcd.print("Fri");
     break;
     case 6:
     lcd.print("Sat");
     break;
     case 0:
     lcd.print("Sun");
     
     break;}
  
  //Every minute update 
  delay(6000);
}
