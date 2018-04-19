void motorLoop()
{
  // motorControl = 5
  analogWrite(motorControl, getMotorSpeed());
}

int getMotorSpeed()
{
  int temperature = getTemperature();
  return 255/2;
}

int getTemperature()
{
  int temp = int(((5.0*analogRead(A0))/1024)*100.0);
  Serial.print("Temperature: ");
  Serial.println(temp);
  return temp;
}

