/*
   Controls the system, comprised of two servos and one motor.
   (x is forward/backwards, y is left/right, z is up/down)
   Servo 1 rotates 180 degrees about its z-axis.
   Servo 2 is rotated by Servo 1 and rotates about its y-axis.
   The motor drives a fan blade and is attached to Servo 2.
   With this setup the motor can be rotated through a hemisphere.
*/

#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial Bluetooth(2, 3); // RX = 2, TX = 3

int numSensors = 1;

int trigs[numSensors] = [0] // pins for triggers of sensors
int echoes[numSensors] = [0] // pins for echoes of sensors
int yDist[numSensors] = [0] // dist in cm of the sensors' y-position

Servo servo_1; // servo rotating along z-axis
Servo servo_2; // servo rotating along y-axis

int pos_1 = 0; // position of Servo 1
int pos_2 = 0; // position of Servo 2

const int MAX_UP = 120; // Servo 1 can be a max of 120 degrees up from straight down
const int MAX_DOWN = 60; // Servo 1 can be a minimum of 60 degrees up from straight down

const int MAX_LEFT = 0; // Servo 2 can be a minimum of 0 degrees from the right
const int MAX_RIGHT = 180; // Servo 2 can be a max of 180 degrees from the right

const int MIN_DIST = 30; // minimum distance to consider the sensor firing

int motorControl = 11; // motor attached to pin 11

int X = 0; // x-position of subject
int Y = 0; // y-position of subject

void setup()
{
  servo_1.attach(9); // Servo 1 set to pin 9
  servo_2.attach(10); // Servo 2 set to pin 10
  pinMode(motorControl, OUTPUT);



  Serial.begin(38400); // receiving data from bluetooth at 9600 bits per second
  Serial.println("Waiting commands....");
  Bluetooth.begin(38400);
  Bluetooth.println("Send 1 to turn on the LED. Send 0 to turn Off");
}

void loop()
{  
  // cycle through the sensors to check which one is firing block
  for (int i = 0; i < numSensors; i++)
  {
    if (dist(trigs[i], echoes[i]) > minDist)
    {
      X = dist(trigs[i], echoes[i]);
      Y = yDist[i];
      break;
    }
  }

  // move the 2nd servo into position
  servo_2.write(angleRotate(X, Y));
  


  // bluetooth communications block
  char c;
  if (Bluetooth.available())
  {
    c = Serial.read(); // reads data from serial, stores it inside character variable
  }

  // the problem is either in the conncection, reading the messages, or in this switch statement
  switch (c)
  {
      if (c == 'l')
      {
        Serial.print("Left received");
        if (pos_2 > MAX_LEFT)
        {
          pos_2 += 15;
          servo_2.write(pos_2);
        }
        break;
      }

      if (c == 'r')
      {
        Serial.print("Right received");
        if (pos_2 < MAX_RIGHT)
        {
          pos_2 -= 15;
          servo_2.write(pos_2);
        }
        break;
      }

      if (c == 'u')
      {
        Serial.print("Up received");
        if (pos_1 < MAX_UP)
        {
          pos_1 += 15;
          servo_1.write(pos_1);
        }
        break;
      }

      if (c == 'd')
      {
        Serial.print("Down received");
        if (pos_1 > MAX_DOWN)
        {
          pos_1 -= 15;
          servo_1.write(pos_1);
        }
        break;
      }
  }
}


// provides the distance reading from one sensor in cm
// takes the average of 10 readings, throwing away
int dist(int trig, int echo)
{
  int d_cm = 0;
  int duration = 0;
  int distances[5];
  int arraysize = sizeof(distances) / sizeof(int);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


  for (int i = 0; i < arraysize; i++)
  {
    digitalWrite(trigPin, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    d_cm = (duration / 2) / 29.1;
    distances[i] = d_cm;
  }

  int sum = 0;
  int n = 0;

  for (int i = 0; i < arraysize; i++)
  {
    if (distances[i] < 3000)
    {
      sum += distances[i];
      n++;
    }
  }

  d_cm = (int)(sum / n);

  return d_cm;
}

// provides the angle for the servo to rotate through
int angleRotate(int x, int y)
{
  
}

