Motion controler code for the RPI Pico (RP2040)
===============================================

This project is a motion controller firmware for the RPI Pico (RP2040), designed for plug-and-play use with modular code and a clear architecture.


I2C description
-----------------------------------------------

The robot’s I2C communication works as follows:
 * Send the device address + R/W bit (to select read or write mode).
 * Send the target register address (to read from or write to).
 * Read or write the register data. Multiple registers can be read/written sequentially, with the address auto-incrementing after each operation.

This firmware is coded to be a slave when adressed. Its address is **0x09** which you can change if there is any conflict whit other hardware.

|Adress |R/W|Description                    |Encoding           |
|-------|:-:|-------------------------------|:-----------------:|
| 0x00  | W | Speed motor 1                 |**-128** - **127** |
| 0x01  | W | Speed motor 2                 |**-128** - **127** |
| 0x02  | W | Speed motor 3                 |**-128** - **127** |
| 0x03  | W | Speed motor 4                 |**-128** - **127** |
| 0x04  | W | Servo 1 position selection    |   **0** - **1**   |
| 0x05  | W | Servo 2 position selection    |   **0** - **1**   |
| 0x06  | W | Servo 3 position selection    |   **0** - **1**   |
| 0x07  | W | Servo 4 position selection    |   **0** - **1**   |


Motors communication description
-----------------------------------------------

Motors are «connected» to their respective I2C buffer address.

To control a motor you need to write data to its adress of the form :

>```C
>int8_t speed;
>```

Value goes from **-128** to **127**.


Servo motors communication description
-----------------------------------------------

Servo motors are «connected» to their respective I2C buffer address.

To control a servo motor you need to write data to its adress of the form :

>```C
>uint8_t close;
>```

Value is 0 or 1 for the open pos or the close pos.


Pinout description
-----------------------------------------------

|Pin |Description                       |GPIO Type  |
|----|----------------------------------|-----------|
| 0  | Motor1 Speed                     | PWM       |
| 1  | Motor2 Speed                     | PWM       |
| 2  | Motor3 Speed                     | PWM       |
| 3  | Motor4 Speed                     | PWM       |
| 4  | Motor1 Dir1                      | OUTPUT    |
| 5  | Motor1 Dir2                      | OUTPUT    |
| 6  | Motor2 Dir1                      | OUTPUT    |
| 7  | Motor2 Dir2                      | OUTPUT    |
| 8  | Motor3 Dir1                      | OUTPUT    |
| 9  | Motor3 Dir2                      | OUTPUT    |
| 10 | Motor4 Dir1                      | OUTPUT    |
| 11 | Motor5 Dir2                      | OUTPUT    |
| 12 | Servo1 Angle                     | PWM       |
| 13 | Servo2 Angle                     | PWM       |
| 14 | Servo3 Angle                     | PWM       |
| 15 | Servo4 Angle                     | PWM       |
| 21 | I2C Bus SDA                      | I2C       |
| 20 | I2C Bus SCL                      | I2C       |
