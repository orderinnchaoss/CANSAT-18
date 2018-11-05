#include<Servo.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <DS3231.h>
#include<EEPROM.h>
#include<math.h>

struct Packet
{
   int TEAM_ID=2426,MISSIONTIME=0,COUNT=0,ALT=0,PRES=0,TEMP=0,VOLT=9,GPSALT=0,SAT=7,STATE=0,HOUR,MIN,SEC;
   double GPSLAT = 00.00,GPSLON = 00.00,TX=0,TY=0,TZ=0;
}; 
Packet In1; 
String pac;

void PactoStr()
{
  pac = String(In1.TEAM_ID) + ',' + String(In1.MISSIONTIME) + ',' + String(In1.COUNT) + ',' + String(In1.ALT) + ',' + String(In1.PRES)  + ',' + String(In1.TEMP)+ ','  + String(In1.VOLT) + ','
             +  String(In1.HOUR) + ':'   + String(In1.MIN) + ':' + String(In1.SEC) + ','
             + String(In1.GPSLAT)+ ',' + String(In1.GPSLON)  +  ',' + String(In1.GPSALT) + ',' + String(In1.SAT)  +  ',' 
             + String(In1.TX) +  ',' + String(In1.TY) +  ',' + String(In1.TZ) +  ',' 
             + String(In1.STATE);
}


float tilt(float X,float Y,float Z)
{
   double a1x = (Y*Y + Z*Z);
  double a2x = sqrt(a1x);
  double ax=atan2(X,a2x);
  return ax;
}

void deploy()
{
  for(pos =180 ; pos >= 90 ; pos--)
  {
   dep.write(pos);
   delay(15);
  }
}

void releaze()
{
  for(pos =90 ; pos >= 5 ; pos--)
  {
   dep.write(pos);
   delay(15);
  }

  for(pos =0 ; pos < 180 ; pos++)
  {
   rel.write(pos);
   delay(1);
  }
 }

void reset()
{
   for(pos = 180 ; pos >= 0 ; pos--)
  {
   rel.write(pos);
   delay(1);
  }

  for(pos = 180 ; pos >= 0 ; pos--)
  {
   dep.write(pos);
   delay(1);
  }
}

bool count()
{ Time t;
   
   t = rtc.getTime();
   int sect = t.sec;
   
   if(sect!=s)
   { s = sect;
     return 1;
     
   }
   return 0;
  }

int s;   
DS3231  rtc(SDA, SCL); //RTC Class
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);




void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
