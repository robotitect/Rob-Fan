/*
  Controls the system, comprised of two servos and one motor.
  (x is forward/backwards, y is left/right, z is up/down)
  [UNUSED] Servo 1 rotates 180 degrees about its z-axis.
  Servo 2 is rotates about y-axis.
  The motor drives a fan blade and is attached to Servo 2.
  The position of a SUBJECT is constantly tracked with +NUM_SENSORS+
  number of ultrasound distance sensors.
  This runs on a standard Arduino Uno Rev3.
*/

#include <math.h>
#include <NewPing.h>
#include <QuickStats.h>
#include <Servo.h>

const int NUM_SENSORS = 2;

int trigs[NUM_SENSORS] = {6, 3}; // Pins for triggers of sensors
int echoes[NUM_SENSORS] = {7, 11}; // Pins for echoes of sensors
int yDist[NUM_SENSORS] = {30, 90}; // Distance in cm of the sensors' y-position from fan

const int MIN_DIST = 10; // Minimum distance to consider the sensor firing
const int MAX_DIST = 100; // This is too far, indicates an error reading from sensor

NewPing sonars[NUM_SENSORS] = {NewPing(6, 7, MAX_DIST), NewPing(3, 11, MAX_DIST)};

Servo servo_1; // [UNUSED] Servo rotating along z-axis
int pin_servo_1 = 13; // [UNUSED]
Servo servo_2; // Servo rotating along y-axis
int pin_servo_2 = 9;

int pos_1 = 0; // [UNUSED] Position of Servo 1
int pos_2 = 0; // Position of Servo 2

const int MAX_UP = 120; // [UNUSED] Servo 1 can be a max of 120 degrees up from straight down
const int MAX_DOWN = 60; // [UNUSED] Servo 1 can be a minimum of 60 degrees up from straight down

const int MAX_LEFT = 0; // Servo 2 can be a minimum of 0 degrees from the right
const int MAX_RIGHT = 180; // Servo 2 can be a max of 180 degrees from the right

int temperaturePin = A0; // [UNUSED] For reading analog from temperature sensor

int motorControl = 5; // Motor attached to PWM pin

int X = 0; // The x-position of SUBJECT
int Y = 0; // The y-position of SUBJECT

int X_old = 0; // Previous x-position of SUBJECT

QuickStats stats; // [UNUSED] Stats helper

void setup()
{
  servo_1.attach(pin_servo_1);
  servo_2.attach(pin_servo_2); // [UNUSED]
  pinMode(motorControl, OUTPUT);
  servo_1.write(0);
  servo_2.write(0); // [UNUSED]

  Serial.begin(38400);
}

void loop()
{
  motorLoop();

  // Cycle through the sensors to check which one is firing
  // [UNADDRESSED] Currently biases the first sensor
  int position = 0;
//  for(int i = 0; i < NUM_SENSORS; i++)
  for(int i = NUM_SENSORS - 1; i > -1; i--)
  {
    X = dist(sonars[i]);
    Serial.println((i + 1) + String(": ") + X);
    if(X > MIN_DIST)
    {
      Y = yDist[i];
      String toWrite = String("X: ") + X + String("cm. Y:") + Y + String("cm.");
      Serial.println(toWrite);
      if(X > 0)
      {
        // Move the 2nd servo into position
        int theta = angleRotate(X, Y);
        if(abs(theta - position) < 45)
        {
          position = theta;
          servo_2.write(theta);
        }
      }
      break;
    }
  }
}


// Provides the distance reading from one sensor in cm
// Returns the average/median of +samples+ readings
int dist(NewPing sonar)
{
  double d_cm = 0;
  int samples = 5;
  float distances[samples];
  int arraysize = sizeof(distances) / sizeof(int);

  for(int i = 0; i < arraysize; i++)
  {
    distances[i] = sonar.ping_cm();
  }

  int sum = 0;
  int n = 0;

  for(int i = 0; i < arraysize; i++)
  {
    if(MIN_DIST < distances[i] && distances[i] < 3000)
    {
      sum += distances[i];
      n++;
    }
  }

  d_cm = (double)(sum/n);
//  Could also use median:
//  d_cm = stats.median(distances, arraysize);
  return (int)d_cm;
}

// Provides the angle for the servo to rotate to
int angleRotate(int y, int x)
{
  double arg = (double) x / y;
  Serial.println(String("Arg: ") + arg);
  int angle = (int)to_degrees(atan(arg));
  Serial.println(String("To rotate servo_2 by ") + angle);
  return angle;
}

inline double to_degrees(double radians)
{
  return (double)radians*(180.0/M_PI);
}
