/*
  Controls the system, comprised of two servos and one motor.
  (x is forward/backwards, y is left/right, z is up/down)
  Servo 1 rotates 180 degrees about its z-axis.
  Servo 2 is rotated by Servo 1 and rotates about its y-axis.
  The motor drives a fan blade and is attached to Servo 2.
  With this setup the motor can be rotated through a hemisphere.
*/

#include <math.h>
#include <NewPing.h>
#include <QuickStats.h>
#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial Bluetooth(2, 3); // RX = 2, TX = 3

const int NUM_SENSORS = 2;

int trigs[NUM_SENSORS] = {6, 3}; // pins for triggers of sensors
int echoes[NUM_SENSORS] = {7, 11}; // pins for echoes of sensors
int yDist[NUM_SENSORS] = {30, 90}; // dist in cm of the sensors' y-position

const int MAX_DIST = 200; // this is too far

NewPing sonars[NUM_SENSORS] = {NewPing(6, 7, MAX_DIST), NewPing(3, 11, MAX_DIST)};

Servo servo_1; // servo rotating along z-axis
int pin_servo_1 = 13; // fix
Servo servo_2; // servo rotating along y-axis
int pin_servo_2 = 9;

int pos_1 = 0; // position of Servo 1
int pos_2 = 0; // position of Servo 2

const int MAX_UP = 120; // Servo 1 can be a max of 120 degrees up from straight down
const int MAX_DOWN = 60; // Servo 1 can be a minimum of 60 degrees up from straight down

const int MAX_LEFT = 0; // Servo 2 can be a minimum of 0 degrees from the right
const int MAX_RIGHT = 180; // Servo 2 can be a max of 180 degrees from the right

const int MIN_DIST = 10; // minimum distance to consider the sensor firing


int temperaturePin = A0;

int motorControl = 5; // motor attached to pwm pin

int X = 0; // x-position of subject
int Y = 0; // y-position of subject

int X_old = 0; // previous x-position of subject

QuickStats stats; // stats helper

void setup()
{
  servo_1.attach(pin_servo_1);
  servo_2.attach(pin_servo_2);
  pinMode(motorControl, OUTPUT);
  servo_1.write(0);
  servo_2.write(0);

  //  for(int i = 0; i < NUM_SENSORS; i++)
  //  {
  //    sonars[i] = new NewPing(trigs[i], echoes[i]);
  //  }

  Serial.begin(38400); // receiving data from bluetooth at 38400 bits per second
  Serial.println("Waiting commands....");
  Bluetooth.begin(38400);
  Bluetooth.println("Send 1 to turn on the LED. Send 0 to turn Off");
}

void loop()
{
  // motor control
  motorLoop();
  
  //  delay(1000);
  // cycle through the sensors to check which one is firing block
  // this thing almost actually works!!!
  // still randomly jumps, but it's the most stable so far
  // now to check if it works with multiple sensors.....
  // UPDATE: it works, but it's noisy
  // need to figure out a way to actually stick with one, currently biases the first sensor
  int position = 0;
  for (int i = 0; i < NUM_SENSORS; i++)
  {
//    X = sonars[i].ping_cm();
    
    X = dist(sonars[i]);
    Serial.println((i + 1) + String(": ") + X);
    //    if (sonars[i].ping_cm() > MIN_DIST)
    if (X > MIN_DIST)
    {
      //      X = sonars[i].ping_cm();
      Y = yDist[i];
      String toWrite = String("X: ") + X + String("cm. Y:") + Y + String("cm.");
      Serial.println(toWrite);
      if (X > 0)
      {
        // move the 2nd servo into position
        int theta = angleRotate(X, Y);

        if (abs(theta - position) < 45)
        {
          position = theta;
          servo_2.write(theta);
        }
      }
      break;
    }
  }





  // SCRAPPED: bluetooth communications block -> end of loop()
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
/*double*/ int dist(NewPing sonar)
{
  double d_cm = 0;
  //  int duration = 0;
  int samples = 1;
  float distances[samples];
  int arraysize = sizeof(distances) / sizeof(int);
  //  pinMode(trigPin, OUTPUT);
  //  pinMode(echoPin, INPUT);


  for (int i = 0; i < arraysize; i++)
  {
    //    digitalWrite(trigPin, LOW);  // Added this line
    //    delayMicroseconds(2); // Added this line
    //    digitalWrite(trigPin, HIGH);
    //    delayMicroseconds(10); // Added this line
    //    digitalWrite(trigPin, LOW);
    //    duration = pulseIn(echoPin, HIGH);
    //    d_cm = (duration / 2) / 29.1;
    //    distances[i] = d_cm;
    distances[i] = sonar.ping_cm();
  }

  int sum = 0;
  int n = 0;

  for (int i = 0; i < arraysize; i++)
  {
    if (MIN_DIST < distances[i] && distances[i] < 3000)
    {
      sum += distances[i];
      n++;
    }
  }

  d_cm = (double)(sum / n);
//  d_cm = stats.median(distances, arraysize);

  return (int)d_cm;
}

// provides the angle for the servo to rotate through
int angleRotate(int y, int x)
{
  double arg = (double) x / y;
  Serial.println(String("Arg: ") + arg);
  int angle = (int) to_degrees(atan(arg));
  Serial.println(String("To rotate servo_2 by ") + angle);
  return angle;
}

inline double to_degrees(double radians)
{
  return (double) radians * (180.0 / M_PI);
}

