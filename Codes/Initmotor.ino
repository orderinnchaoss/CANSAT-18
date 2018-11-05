/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo dep;
Servo rel;// create servo object to control a servo
// twelve servo objects can be created on most boards


int pos = 20;    // variable to store the servo position
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

/*  for(pos =0 ; pos < 180 ; pos++)
  {
   myservo.write(pos);
   delay(1);
  }
  */
  
}



void setup() {
  dep.attach(5); 
  rel.attach(6);// attaches the servo on pin 9 to the servo object
}
void loop()
{
dep.write(20);
  delay(5);
  rel.write(10);
  delay(1000);

 for(int i=20;i<=100;i++)
  {
  dep.write(i);
  delay(15);
  }

 delay(1000);

 for(int i=100;i<=180;i++)
 {
  dep.write(i);
  delay(15);
 }

 delay(1000);
  
  
}

