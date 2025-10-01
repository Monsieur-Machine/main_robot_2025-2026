Motion controler code for the RPI Pico (RP2040)
===============================================

This project is the main controller firmware for the RPI Pico (RP2040), designed for the Eurobot 2025 Cup.


I2C description
-----------------------------------------------

The robot’s I2C communication works as follows:
 * Send the device address + R/W bit (to select read or write mode).
 * Send the target register address (to read from or write to).
 * Read or write the register data. Multiple registers can be read/written sequentially, with the address auto-incrementing after each operation.

This code is designed to be the master in the i2c communication.

|Register |R/W|Description                    |Encoding           |
|---------|:-:|-------------------------------|:-----------------:|
| 0x00    | W | Speed motor 1                 |**-128** - **127** |
| 0x01    | W | Speed motor 2                 |**-128** - **127** |
| 0x02    | W | Speed motor 3                 |**-128** - **127** |
| 0x03    | W | Speed motor 4                 |**-128** - **127** |
| 0x04    | W | Servo 1 position selection    |   **0** - **1**   |
| 0x05    | W | Servo 2 position selection    |   **0** - **1**   |
| 0x06    | W | Servo 3 position selection    |   **0** - **1**   |
| 0x07    | W | Servo 4 position selection    |   **0** - **1**   |


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

Value is 0 or 1 for the open or the close pos.


Internet communication description
-----------------------------------------------

The robot main_controller is a client connected to the wireless controller which is an udp server host. A buffer is used to store data received from host.

Speed on X and Y axis are not depending of the robot orientation.

Servo motors keep the same byte address in i2c buffer and udp payload.

|Byte     |Description                                      |Encoding           |
|---------|-------------------------------------------------|:-----------------:|
| 0x00-01 | Robot angle (0x00 is the last significant byte) |**-180** - **180** |
| 0x02    | Speed x axis                                    |**-128** - **127** |
| 0x03    | Speed y axis                                    |**-128** - **127** |
| 0x04    | Servo 1 position selection                      |   **0** - **1**   |
| 0x05    | Servo 2 position selection                      |   **0** - **1**   |
| 0x06    | Servo 3 position selection                      |   **0** - **1**   |
| 0x07    | Servo 4 position selection                      |   **0** - **1**   |


Pinout description
-----------------------------------------------

|Pin |Description                       |GPIO Type  |
|----|----------------------------------|-----------|
| 4  | I2C Bus SDA                      | I2C       |
| 5  | I2C Bus SCL                      | I2C       |


Motors placement
-----------------------------------------------

    ,-~***~-,
  /1         2\
 |             |
 |             |
 |             |
  \3         4/
    `-.....-'
