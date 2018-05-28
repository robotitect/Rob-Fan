# Rob|Fan

**Rob|Fan*** is a fan that follows a subject around a room,
always pointing its breeze straight at them: a **rob**otic **fan**.

## Technical Details

<image-of-whiteboard-explanation>

An array of sensors lies along one wall. Each of their positions *with
respect to the fan* is known beforehand and stored in the array
`yDist[NUM_SENSORS]` in `./sketch/sketch.ino/sketch.ino.ino`
(I deeply apologize for this naming convention).
`loop()` checks each sensor in turn to see if a valid distance is being returned,
which means that someone is standing in front of the sensor.
We reference the specific sensor's position *from the fan*
in order to come up with a **y-coordinate**,
and we use the distance received from the sensor as the **x-coordinate**.
Then we use math:

```cpp
// SUCCINCT
int angleRotate(int x, int y)
{
  return (int) atan((double) x / y);
}
```
This gives the angle *from the
[wall of sensors/wall intersecting Fan & parallel to sensor lines]*
to rotate the servo towards.

## Major Components

While some of these aren't the **exact** ones I used they should be
*close enough*.
- [x] [Arduino Uno Rev3](https://store.arduino.cc/usa/arduino-uno-rev3) (1x)
- [x] [HC-SR04 Ultrasonic Distance Sensor](https://www.sparkfun.com/products/13959) (2x)
- [x] [Micro Servo Motor](https://www.sparkfun.com/products/9065) (1x)
- [x] [DC Hobby Motor](https://www.sparkfun.com/products/11696) (1x)
- [x] [Propellor](https://www.amazon.com/Flormoon-Propeller-Four-vane-Leaves-Airplane/dp/B06XCTGCLK) (1x)

And these were some possible extensions:
- [ ] [HC-SR04 Ultrasonic Distance Sensor](https://www.sparkfun.com/products/13959) (More than 2x)
- [ ] [Temperature Sensor](https://www.sparkfun.com/products/10988) (1x)
- [ ] [Bluetooth Module](https://www.amazon.com/HiLetgo%C2%AE-Wireless-Bluetooth-Transceiver-Arduino/dp/B071YJG8DR/ref=sr_1_3?s=electronics&ie=UTF8&qid=1527396105&sr=1-3&keywords=hc-05) (1x)

**(Yes, this name is inspired by ~~stolen from~~ [Rob|Arch](http://www.robarch2018.org/))*
