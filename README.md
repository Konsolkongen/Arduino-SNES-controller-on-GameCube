# Arduino SNES controller on GameCube
Modify a SNES (or similar) controller to work on GameCube using an Arduino. The goal was to make a cheap alternative to the expensive Hori Gameboy controller using easily available parts.

Based on the Smash Box code by SimpleControllers: https://github.com/SimpleControllers/SimpleControllersBuild-a-Box
Requires the Nintendo library by NicoHood: https://github.com/NicoHood/Nintendo

# Features
* Switch between D-pad and analog mapping
* Two different button layouts
* Can save current settings to be automatically re-enabled on power on

Press and hold Select, L and R together with the button of choice for about one second to set the current mode. D-pad assignment and button layouts can be set individually.
![SNES controller](/SNES_controller.gif)

(Save combo must be held for 2-3 second).

The regular button layout matches the buttons on the SNES controller. The alternate button layout tries to mimic the layout of a GameCube controller which should work better for Gameboy Advance games, it's easier to jump while running in Super Mario.

Controller buttons | Regular layout | Alternate layout
------------ | ------------- | -------------
A | A | X
B | B | A
X | X | Y
Y | Y | B

# Required components
* Controller to sacrifice
* A compatible Arduino board @16MHz, I use an Arduino Pro Mini (must be powered by 3.3V!)
* GameCube controller extension cord (wire colors might be different from mine, check continuity yourself)
* 10k resistor to pull up the data line

To my knowledge no 3.3V 16Mhz Arduino board exist, but the ATmega328p on a 5V 16Mhz Arduino Pro Mini can run at 3.3V. Though it is not advised to do so, I personally haven't had any issues, but please keep this in mind. Additional information can be found in the microcontroller datasheet:
http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf

# How to build (will be improved eventually)
* Flash the code to the Arduino. An external programmer like the USBtinyISP works great.
* Remove all components on the controller PCB and solder the button outputs of the Arduino to the controller PCB. Don't forget to connect GND.
* Wire up 3.3V, GND and Data from the extension cord. DO NOT USE 5V!
* Connect the 10k resistor between 3.3V and the data line.

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

The Pro Mini fits nicely in this spot:
![SNES controller](/pictures/placement.jpg)

You can wire to the pads where the old chip used to be for a neater result:
![SNES controller](/pictures/wiring.jpg)

# Known issues
The code runs quite slow on the Pro Mini. Not sure what the issue is, but that explains why I don't know the exact time you must hold the button combos. This hasn't posed any issues at all when playing, but it should be still be mentioned.

The Qoob Pro BIOS absolutely hates this controller. It either doesn't work right or it will just refuse to boot. This is unfortunate but once you're past that I've experienced no issues.
