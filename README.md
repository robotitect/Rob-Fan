Rob-Fan
==================
The goal of this is to create a fan that follows a subject around a room, always pointing its breeze straight at them: a *robotic fan*. It's being built up in 3 phases: 
1. *Electromechanical*: Putting together the circuitry that it runs on.
2. *Smart/IoT*: Putting together an Android app that monitors and provides override control for the fan.
3. *Robotic*: Using sensors to track a subject's position in a room, and moving the fan accordingly.
## Electromechanical
The fan consists of two servos attached to eachother, with the one controlling movement in the up/down direction, and the other controlling movement in the left-right direction. Attached to these is a motor that drives a fan blade. Mostly complete.
## Smart/IoT
The current phase, where an app is being developed to monitor/control the servos over Bluetooth via an HC-05 module.
## Robotic
A future addition where the fan becomes entirely autonomous, controlling its rotation and blade speed based entirely on environmental factors. The app remains as an IoT component allowing for emergency override.