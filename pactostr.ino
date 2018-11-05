#include<EEPROM.h>
struct Packet
{
  int TEAM_ID = 2426, MISSIONTIME = 0, COUNT = 0, SAT = 0, STATE = 0, HOUR, MIN, SEC;
  double GPSLAT = 32.21, GPSLON = -98.20, TX = 0, TY = 0, TZ = 0, ALT = 0, TEMP = 0, VOLT = 9, GPSALT = 0;
  long PRES = 0;
};
Packet In1,In2; 
String pac;
void setup(){

 Serial.begin(9600);
}
void PactoStr()
{
  pac = String(In1.TEAM_ID) + ',' + String(In1.MISSIONTIME) + ',' + String(In1.COUNT) + ',' + String(In1.ALT) + ',' + String(In1.PRES)  + ',' + String(In1.TEMP)+ ','  + String(In1.VOLT) + ','
             +  String(In1.HOUR) + ':'   + String(In1.MIN) + ':' + String(In1.SEC) + ','
             + String(In1.GPSLAT)+ ',' + String(In1.GPSLON)  +  ',' + String(In1.GPSALT) + ',' + String(In1.SAT)  +  ',' 
             + String(In1.TX) +  ',' + String(In1.TY) +  ',' + String(In1.TZ) +  ',' 
             + String(In1.STATE);
}
int Addr=0; 
void loop(){
 
  delay(1000);
  EEPROM.get(Addr,In1);
   PactoStr();
  Serial.println(pac);
  Addr = Addr + sizeof(In1);
  if(Addr >= 870)
   Addr=0;
  /*Serial.println(sizeof(In1));
  EEPROM.get(945,In1);
  PactoStr();
  Serial.println(pac);*/
 // delay(10000);
  
}
