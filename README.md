# PIC32MZ EF Starter Kit | DM320007C #

This project is intended to be a clean-slate starting point for using the DM320007/C development
board from Microchip. The entire project folder is available to import into MPLABX. After
connecting your development board, you should be able to compile and program.

### Operation ###

For fast development and rapid debugging, the program initializes UART2 at 115200 baud to
communicate with the outside world. By default, when including <stdio.h> in any MPLABX
project, the library will select UART2 to print to. Fortunately, the DM320007C has an
MCP2221 USB-UART brige on board - plug the device in, install the drivers, and open
your favorite terminal program. From there, printf() will print to the PC terminal.

The program also provides a basic example on how to initialize and execute ISRs. It
initializes Timer1 to flash the green LED on RH2 using a timer interrupt.

I've also added a basic I2C driver which interfaces with an optional EEPROM. It uses
I2C2 to read/write/erase the device.

### Setup ###

This project should be a direct import to MPLABX. After compiling and programming
your DM320007/C:

* Open a terminal program. (I used Windows Powershell)
* Find the COM port associated with the MCP2221.
* Open that COM port: 115200 baud, 8 bits, 1 stop bit, NO parity

Perform reads from the COM port, you should see "Hello World!" print rapidily. A
Windows Powershell script file is provided to perform constant reads on the port.
However, you may need to change the 'COM4' depending on how Windows decided to
enumerate your development board.
