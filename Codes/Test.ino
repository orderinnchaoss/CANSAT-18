
#include <SFE_BMP180.h>
#include <Wire.h>
#include<EEPROM.h>
#include <Servo.h>
#include<math.h>
#include <DS3231.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

struct Packet
{
  int TEAM_ID = 2426, MISSIONTIME = 0, COUNT = 0, SAT = 0, STATE = -1, HOUR, MIN, SEC;
  double GPSLAT = 32.21, GPSLON = -98.20, TX = 0, TY = 0, TZ = 0, ALT = 0, TEMP = 0, VOLT = 9, GPSALT = 0;
  long PRES = 0;
};
Packet In1;
String pac;
int Addr = 0;

int y;         // Tilt
int z;         // Tilt
int x;         // Tilt
int pos;
int buz = 1000;
Servo dep;
Servo rel;
DS3231  rtc(SDA, SCL);
TinyGPSPlus gps;
SoftwareSerial xbee(2, 9);

void tempres();
static void smartDelay(unsigned long ms);
float tilt(float , float , float );
void deploy();
void releaze();
void delay1s();
void PactoStr();

SFE_BMP180 pressure;

double baseline;

static const int RXPin = 7, TXPin = 8;
static const uint32_t GPSBaud = 9600;

SoftwareSerial ss(RXPin, TXPin);

void setup()
{ Serial.begin(9600);
  xbee.begin(9600);
  ss.begin(GPSBaud);
  dep.attach(6);
  rel.attach(5);
  pinMode(A0, INPUT);
  pinMode(A3, INPUT);
  pinMode(A2, INPUT);
  pinMode(A1, INPUT);
  pinMode(11, OUTPUT);
  rtc.begin();


  if (pressure.begin())
    baseline = getPressure();

}
double amax = 0;
int depl = 0;
int rels = 0;



void loop()
{ xbee.end();
  ss.end();
  tone(3, buz);
  if (In1.STATE == -1)
  {
    In1.STATE = 0;
    dep.write(140);
    delay(5);
    rel.write(0);
    delay(10000);

    for (int i = 140; i >= 77; i--)
    {
      dep.write(i);
      delay(15);
    }

    delay(10000);

    for (int i = 77; i >= 10; i--)
    {
      dep.write(i);
      delay(15);

    }
    delay(10000);

    dep.detach();
    rel.detach();
    xbee.begin(9600);
    ss.begin(9600);
  }
  double a, P;
  digitalWrite(11, HIGH);
  P = getPressure();
  a = pressure.altitude(P, baseline);
  In1.ALT = a;
  smartDelay(0);
  if (gps.satellites.isValid())
    In1.SAT = gps.satellites.value();
  else
    In1.SAT = 0;
  Serial.println(analogRead(A0));
  float w = analogRead(A0) * 55;
  In1.VOLT = (w / 1023)*2.5;


  smartDelay(0);
  if (gps.location.isValid())
  {
    In1.GPSLAT = gps.location.lat();
    In1.GPSLON = gps.location.lng();
  }
  smartDelay(0);
  if (gps.time.isValid())
  {


    if (gps.time.hour() < 5)
      In1.HOUR = (24 + gps.time.hour() - 5);
    else
      In1.HOUR = gps.time.hour() - 5;
    In1.MIN = gps.time.minute();
    In1.SEC = gps.time.second();
  }
  else

  { Time t = rtc.getTime();
    In1.HOUR = (t.hour);
    In1.MIN = (t.min);
    In1.SEC = (t.sec);
  }



  smartDelay(0);
  if (gps.altitude.isValid())
    In1.GPSALT = gps.altitude.meters();
  else
    In1.GPSALT = a;

  x = analogRead(A1);
  y = analogRead(A2);
  z = analogRead(A3);

  double Z = map(z, 55, 555, -1500, 1500);
  double Y = map(y, 124, 641, -1500, 1500);
  double X = map(x, 94, 661, -1500, 1500);

  In1.TX = (tilt(X, Y, Z));
  In1.TY = (tilt(Z, Y, X));
  In1.TZ = (tilt(Y, X, Z));
  tempres();

  if (a > amax)
    amax = a;
  if (amax >= 651 && a <= 630)
  {
    deploy();
    In1.STATE = 1;
  }
  if (amax >= 651 && a <= 300)
  {
    releaze();
    In1.STATE = 2;
  }
  if (amax >= 651 && a <= 5)
  {
    buz = 1500;
    In1.STATE = 3;
  }

  delay1s();
  In1.MISSIONTIME++;
  In1.COUNT++;
  PactoStr();
  Serial.println(pac);
  xbee.println(pac);
  EEPROM.put(Addr, In1);
  Addr = Addr + sizeof(In1);
  if (Addr >= 950)
    Addr = 0;

}


void tempres()
{
  char status;
  double T, P;

  status = pressure.startTemperature();
  if (status != 0)
  {

    delay(status);


    status = pressure.getTemperature(T);
    if (status != 0)
    {

      In1.TEMP = T;


      status = pressure.startPressure(3);
      if (status != 0)
      {

        delay(status);


        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          In1.PRES = P * 100;
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
}

float tilt(float X, float Y, float Z)
{
  double a1x = (Y * Y + Z * Z);
  double a2x = sqrt(a1x);
  double ax = atan2(X, a2x);
  return 57.21 * ax;
}

double getPressure()
{
  char status;
  double T, P;
  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          return (P);
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
}

void deploy()
{ xbee.end();
  ss.end();
  dep.attach(6);
  delay(7);
  for (pos = 10 ; pos <= 95 ; pos++)
  {
    dep.write(pos);
    delay(17);
  }
  delay(17);
  dep.detach();
  xbee.begin(9600);
  ss.begin(9600);
}


void releaze()
{ xbee.end();
  ss.end();
  dep.attach(6);
  rel.attach(5);
  delay(7);
  for (pos = 95 ; pos <= 180 ; pos++)
  {
    dep.write(pos);
    delay(17);
  }
  for (pos = 0 ; pos <= 180 ; pos++)
  {
    rel.write(pos);
    delay(17);
  }
  delay(17);
  rel.detach();
  dep.detach();
  xbee.begin(9600);
  ss.begin(9600);
}

void delay1s()
{
  Time t = rtc.getTime();
  int Sect = t.sec;
  while (Sect == t.sec)
    t = rtc.getTime();
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

void PactoStr()
{
  pac = String(In1.TEAM_ID) + ',' + String(In1.MISSIONTIME) + ',' + String(In1.COUNT) + ',' + String(In1.ALT) + ',' + String(In1.PRES)  + ',' + String(In1.TEMP) + ','  + String(In1.VOLT) + ','
        +  String(In1.HOUR) + ','   + String(In1.MIN) + ',' + String(In1.SEC)   + ','
        + String(In1.GPSLAT) + ',' + String(In1.GPSLON)  +  ',' + String(In1.GPSALT) + ',' + String(In1.SAT)  +  ','
        + String(In1.TX) +  ',' + String(In1.TY) +  ',' + String(In1.TZ) +  ','
        + String(In1.STATE) ;
}

