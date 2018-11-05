#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;


SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

void loop()
{
  
  smartDelay(0);
  if(gps.satellites.isValid())
   Serial.println(gps.satellites.value());

  smartDelay(0);
  if(gps.location.isValid())
   {
    Serial.println(gps.location.lat());
    Serial.println(gps.location.lng());
    
   }
   smartDelay(0);
   if(gps.time.isValid())
    {
      Serial.print(gps.time.hour());
      Serial.print("/");
      Serial.print(gps.time.minute());
      Serial.print("/");
      Serial.println(gps.time.second());
     }
  

    smartDelay(0);
    if(gps.altitude.isValid());
      Serial.println(gps.altitude.meters()); 
     
   }
   


