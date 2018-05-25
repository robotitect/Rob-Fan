// Controls the motor
void motorLoop()
{
  // motorControl = 5
  analogWrite(motorControl, getMotorSpeed());
}

// Provides the spped for the motor
// Would have used the temperature sensor; mapped temperature -> speed level
int getMotorSpeed()
{
  int temperature = getTemperature();
  return 255/2;
}

// [UNUSED] Reads temperature from temp. sensor
int getTemperature()
{
  int temp = int(((5.0*analogRead(A0))/1024)*100.0);
  Serial.print("Temperature: ");
  Serial.println(temp);
  return temp;
}

