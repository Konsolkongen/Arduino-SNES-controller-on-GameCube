/* SNES controller for GameCube, Wii, Wii U* and Switch** v2.3.
Uses the Nintendo library by Nicohood.
Based on code by Simple Controllers.
 
What it does:
Use an Arduino as a controller for your GameCube. Any 16MHz Arduino should work as long as it has enough I/O. 
In my case I used an Arduino Pro Mini (5V, 16MHz) as they are cheap and have enough I/O to adapt this into a SNES controller.
 
Can switch between dpad and left analog stick mappings, as well as two different button layouts.
The normal button mode is the standard Super Nintendo layout, the alternative one tries to mimmick the GameCube button arrangement instead. In many cases this works better for GBA-games.

To switch between these states press and hold Select (Z), L and R and press one of the following buttons for about a second:
A: Regular button mapping (SNES style)
B: Alternate button mapping (GameCube style)
Down: Dpad
Up: Left analog stick

Start: Saves the button and dpad states to EEPROM, and are loaded each time the controller is plugged in. (This button combination must be held for 2-3 seconds).

NOTE: The very first time the controller is plugged in, both states are unknown and you will need to map both the dpad and buttons to one of the options described above, and remember to save. After this the controller will function normally every time it's plugged in.

* For Wii U you will need to use a Smash Bros adapter. For this controller to work with other games besides SSB you will probably have to softmod your Wii U.
** For Switch you will also need the Smash Bros adapter. This should work for all games, but as no GameCube controllers have a Select-button this button will be unavailable.

Konsolkongen 2018, 2020 */

 #include "Nintendo.h"
 #include <EEPROM.h>
//This makes the controller bidirection data line on pin number 17.
CGamecubeConsole GamecubeConsole(17);      //Define a pin for sending data to the GameCube (connect the 'data' wire here).
Gamecube_Data_t d = defaultGamecubeData;   //Structure for data to be sent to console.

//This is needed but you don't have to connect anything to this pin.
CGamecubeController GamecubeController1(15);

//Reference the GameCube controller port pinout and connect Data, 3.3v and Ground. The remaining wires are not needed.

//This is the pinout of the controller. Can be changed to your liking, i.e. "const int A = 4" means that the A button is wired to pin 4 of the Arduino.
const int buttonA = 4;
const int buttonB = 5;
const int buttonX = 3;
const int buttonY = 6;
const int buttonZ = 8; //Select on the SNES controller.
const int buttonSTART = 7;
const int buttonL = 0;
const int buttonR = 2;
const int buttonLEFT = 10;
const int buttonRIGHT = 9;
const int buttonUP = 1;
const int buttonDOWN = 14;

int EEPROMbutton = 10; // Saves the button mapping setting to an addess in EEPROM.
int EEPROMdpad = 11; // Saves the dpad/analog setting to an addess in EEPROM.
int buttonMode; // 0 = Normal, 1 = GameCube layout
int dpadMode; // 0 = D-pad, 1 = Analog

int buttonAstate;
int buttonBstate;
int buttonUpState;
int buttonDownState;
int buttonStartState;

int lastButtonAstate;
int lastButtonBstate;
int lastButtonUpState;
int lastButtonDownState;
int lastButtonStartState;

unsigned long timer;   // Variable that stores time.

void setup()
{
//  Serial.begin(9600);

  //This is needed to run the code.
  GamecubeController1.read();
  
  //Pulls the input pins high.
  pinMode(buttonA, INPUT_PULLUP);
  pinMode(buttonB, INPUT_PULLUP);
  pinMode(buttonX, INPUT_PULLUP);
  pinMode(buttonY, INPUT_PULLUP);
  pinMode(buttonZ, INPUT_PULLUP);
  pinMode(buttonSTART, INPUT_PULLUP);  
  pinMode(buttonL, INPUT_PULLUP);
  pinMode(buttonR, INPUT_PULLUP);  
  pinMode(buttonLEFT, INPUT_PULLUP);
  pinMode(buttonRIGHT, INPUT_PULLUP);
  pinMode(buttonUP, INPUT_PULLUP);
  pinMode(buttonDOWN, INPUT_PULLUP);

  buttonMode = EEPROM.read(EEPROMbutton);
  dpadMode = EEPROM.read(EEPROMdpad);
}

void loop()
{
buttonAstate = digitalRead(buttonA);
buttonBstate = digitalRead(buttonB);
buttonUpState = digitalRead(buttonUP);
buttonDownState = digitalRead(buttonDOWN);
  
//  Serial.print("Z: ");
//  Serial.print(buttonZstate);
//  Serial.print(" L: ");
//  Serial.print(digitalRead(buttonL));
//  Serial.print(" R: ");
//  Serial.print(digitalRead(buttonR));
//  Serial.print(" DOWN: ");
//  Serial.print(digitalRead(buttonDOWN));
//  Serial.print(" First time: ");
//  Serial.print(timer);
//  Serial.print(" Millis: ");
//  Serial.print(millis());
//  Serial.print(" ");
//  Serial.print(millis_held);
//  Serial.print("      Button mode: ");
//  Serial.print(buttonMode);
//  Serial.print("      Dpad mode: ");
//  Serial.println(dpadMode);
  
// This resets and establishes all the values after the controller sends them to the console and helps with initial "zeroing".
int pinA = 0;
int pinB = 0;
int pinX = 0;
int pinY = 0;
int pinZ = 0;
int pinSTART = 0;

int pinR = 0;
int pinL = 0;

int pinLEFT = 0;
int pinRIGHT = 0;
int pinUP = 0;
int pinDOWN = 0;
 
int pinxAxis = 128;
int xmod = 0;
int pinyAxis = 128;


// If buttons Select (Z), L and R are pressed simultaneously with one of the designated mode buttons, the controller will move current time into a variable.

if (digitalRead(buttonZ) == LOW && digitalRead(buttonL) == LOW && digitalRead(buttonR) == LOW && digitalRead(buttonA) == LOW && lastButtonAstate == HIGH) { 
  timer = millis();
}
  
if (digitalRead(buttonZ) == LOW && digitalRead(buttonL) == LOW && digitalRead(buttonR) == LOW && digitalRead(buttonB) == LOW && lastButtonBstate == HIGH) {
  timer = millis(); 
}

if (digitalRead(buttonZ) == LOW && digitalRead(buttonL) == LOW && digitalRead(buttonR) == LOW && digitalRead(buttonUP) == LOW && lastButtonUpState == HIGH) {
  timer = millis(); 
}

if (digitalRead(buttonZ) == LOW && digitalRead(buttonL) == LOW && digitalRead(buttonR) == LOW && digitalRead(buttonDOWN) == LOW && lastButtonDownState == HIGH) {
  timer = millis(); 
}

if (digitalRead(buttonZ) == LOW && digitalRead(buttonL) == LOW && digitalRead(buttonR) == LOW && digitalRead(buttonSTART) == LOW && lastButtonStartState == HIGH) {
  timer = millis(); 
}

// If the mode button is held for at certain amount of time the controller will switch to a different mapping. The code runs a bit sluggish (Uno and Pro Mini), so the actual time is slower than it appears here.

// Hold with A for normal button mapping.
if (digitalRead(buttonA) == LOW && digitalRead(buttonZ) == LOW && digitalRead(buttonL) == LOW && digitalRead(buttonR) == LOW && millis() - timer >= 500) { // 
  buttonMode = 0;
}

// Hold with B for GameCube button layout.
if (digitalRead(buttonB) == LOW && digitalRead(buttonZ) == LOW && digitalRead(buttonL) == LOW && digitalRead(buttonR) == LOW && millis() - timer >= 500) { // 
  buttonMode = 1;
}

// Hold with Up for Analog stick.
if (digitalRead(buttonUP) == LOW && digitalRead(buttonZ) == LOW && digitalRead(buttonL) == LOW && digitalRead(buttonR) == LOW && millis() - timer >= 500) { // 
  dpadMode = 1;
}

// Hold with Down for Dpad.
if (digitalRead(buttonDOWN) == LOW && digitalRead(buttonZ) == LOW && digitalRead(buttonL) == LOW && digitalRead(buttonR) == LOW && millis() - timer >= 500) { // 
  dpadMode = 0;
}

// Hold with Start for a few seconds to save the current preset. This is then automatically enabled next time the controller is powered on.
if (digitalRead(buttonSTART) == LOW && digitalRead(buttonZ) == LOW && digitalRead(buttonL) == LOW && digitalRead(buttonR) == LOW && millis() - timer >= 1500) { // 
  EEPROM.write(EEPROMbutton, buttonMode);
  EEPROM.write(EEPROMdpad, dpadMode);
}

else if (digitalRead(buttonZ) == HIGH && digitalRead(buttonL) == HIGH && digitalRead(buttonR) == HIGH)  { // If the buttons are released reset the variable to 0.
  timer = 0;
}

  switch (buttonMode) {
    case 0: // Regular mapping - SNES style
      if (digitalRead(buttonA) == LOW)pinA = 1;
      if (digitalRead(buttonB) == LOW)pinB = 1;
      if (digitalRead(buttonX) == LOW)pinX = 1;
      if (digitalRead(buttonY) == LOW)pinY = 1;
    break;
 
    case 1: // Alternate mapping - A = X, B = A, Y = B, X = Y
      if (digitalRead(buttonA) == LOW)pinX = 1;
      if (digitalRead(buttonB) == LOW)pinA = 1;
      if (digitalRead(buttonX) == LOW)pinY = 1;
      if (digitalRead(buttonY) == LOW)pinB = 1;
    break;
  }

  switch (dpadMode) {
    case 0: // D-pad
      if (digitalRead(buttonLEFT) == LOW)pinLEFT = 1;
      if (digitalRead(buttonRIGHT) == LOW)pinRIGHT = 1;
      if (digitalRead(buttonUP) == LOW)pinUP = 1;
      if (digitalRead(buttonDOWN) == LOW)pinDOWN = 1;
    break;

    case 1: // Analog stick
      if (digitalRead(buttonLEFT) == LOW)pinxAxis = 0;
      if (digitalRead(buttonRIGHT) == LOW)pinxAxis = 255;
      if (digitalRead(buttonUP) == LOW)pinyAxis = 255;
      if (digitalRead(buttonDOWN) == LOW)pinyAxis = 0 ;
    break;
  }

  if (digitalRead(buttonZ) == LOW)pinZ = 1;
  if (digitalRead(buttonSTART) == LOW)pinSTART = 1;
  if (digitalRead(buttonR) == LOW)pinR = 1;
  if (digitalRead(buttonL) == LOW)pinL = 1;

  //Reports data.
  d.report.a = pinA;
  d.report.b = pinB;
  d.report.x = pinX;
  d.report.y = pinY;
  d.report.z = pinZ;
  d.report.start = pinSTART;
  d.report.r = pinR;
  d.report.l = pinL;
  d.report.dleft = pinLEFT;
  d.report.dright = pinRIGHT;
  d.report.dup = pinUP;
  d.report.ddown = pinDOWN;
  d.report.xAxis = pinxAxis;
  d.report.yAxis = pinyAxis;

  //Sends the complied data to console when the console polls for input.
  GamecubeConsole.write(d);

  lastButtonAstate = buttonAstate;
  lastButtonBstate = buttonBstate;
  lastButtonUpState = buttonUpState;
  lastButtonDownState = buttonDownState;
  lastButtonStartState = buttonStartState;
}
