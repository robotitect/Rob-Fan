/*
 * Controls the system, comprised of two servos and one motor.
 * (x is forward/backwards, y is left/right, z is up/down)
 * Servo 1 rotates 180 degrees about its z-axis.
 * Servo 2 is rotated by Servo 1 and rotates about its y-axis.
 * The motor drives a fan blade and is attached to Servo 2.
 * With this setup the motor can be rotated through a hemisphere. 
 */

#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial Bluetooth(2,3); // RX = 2, TX = 3

Servo servo_1; // servo rotating along z-axis
Servo servo_2; // servo rotating along y-axis

int pos_1 = 0; // position of Servo 1
int pos_2 = 0; // position of Servo 2

const int MAX_UP = 120; // Servo 1 can be a max of 120 degrees up from straight down
const int MAX_DOWN = 60; // Servo 1 can be a minimum of 60 degrees up from straight down

const int MAX_LEFT = 0; // Servo 2 can be a minimum of 0 degrees from the right
const int MAX_RIGHT = 180; // Servo 2 can be a max of 180 degrees from the right

int motorControl = 11; // motor attached to pin 11

void setup() 
{
  servo_1.attach(9); // Servo 1 set to pin 9
  servo_2.attach(10); // Servo 2 set to pin 10
  pinMode(motorControl, OUTPUT);

  Bluetooth.begin(38400); 
  Serial.begin(38400); // receiving data from bluetooth at 9600 bits per second
  Serial.println("Waiting commands....");
  Bluetooth.println("Send 1 to turn on the LED. Send 0 to turn Off");
}

void loop() 
{

  char c;
  if(Serial.available())
  {
    c = Serial.read(); // reads data from serial, stores it inside character variable
  }

  // the problem is either in the conncection, reading the messages, or in this switch statement
  switch(c)
  {
    case 'l': 
    {
      if(pos_2 > MAX_LEFT)
      { 
        pos_2 += 15;
        servo_2.write(pos_2);
      }
      break;
    }

    case 'r':
    {
      if(pos_2 < MAX_RIGHT)
      {
        pos_2 -= 15;
        servo_2.write(pos_2);
      }
      break;
    }

    case 'u':
    {
      if(pos_1 < MAX_UP)
      {
        pos_1 += 15;
        servo_1.write(pos_1);
      }
      break;
    }

    case 'd':
    {
      if(pos_1 > MAX_DOWN)
      {
        pos_1 -= 15;
        servo_1.write(pos_1);
      }
      break;
    }
  }
}
