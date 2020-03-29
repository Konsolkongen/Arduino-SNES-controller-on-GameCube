# Arduino-SNES-controller-on-GameCube
Modify a SNES (or similar) controller to work on GameCube using an Arduino. The goal was to make a cheap alternative to the expensive Hori Gameboy controller using easily available parts.

Based on the Smash Box code by SimpleControllers: https://github.com/SimpleControllers/SimpleControllersBuild-a-Box
Requires the Nintendo library by NicoHood: https://github.com/NicoHood/Nintendo

# Features
* Switch between D-pad and analog mapping
* Two different button layouts
* Can save current settings to be automatically re-enabled on power on

Press and hold Select, L and R together with the button of choice for about one second to set the current mode. D-pad assignment and button layouts can be set individually.
![SNES controller](/SNES_controller.gif)

The regular button layout matches the buttons on the SNES controller. The alternate button layout tries to mimic the layout of a GameCube controller which might work better for Gameboy Advance games as it should be easier to jump while running in Super Mario using this layout.

Controller buttons | Regular layout | Alternate layout
------------ | ------------- | -------------
A | A | X
B | B | A
X | X | Y
Y | Y | B

# Required components
* Controller to sacrifice
* A compatible Arduino board @16MHz, I use an Arduino Pro Mini (must be powered by 3.3V!)
* GameCube controller extension cord

To my knowledge no 3.3V 16Mhz Arduino board exist, but the ATmega328p on a 5V 16Mhz Arduino Pro Mini can run at 3.3V. Though this is not advised to do so, I personally haven't had any issue doing so, but please keep this in mind. Additional information can be found in the microcontroller datasheet:
http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf

# How to build (will be improved eventually)
* Flash the code to the Arduino. An external programmer like the USBtinyISP works great.
* Remove all components on the controller PCB and solder in the Arduino to the pads where the controller IC used to be.
* Wire up 3.3V, GND and Data from the extension cord. DO NOT USE 5V!

Arduino Pro Mini pinout:

Arduino pin | Function/physical button
------------ | -------------
VCC | 3.3V
GND | GND
17 | Data
3 | X
4 | A
5 | B
6 | Y
7 | Start
8 | Select (Z)
0 | L
2 | R
1 | Up
9 | Right
10 | Left
14 | Down
