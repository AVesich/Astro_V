#include "vex.h"

int currAuto = 0; // Index of currently selected auto
const int MAX_INDEX = 6; // Should always be 1 lower than auto number
const int NUM_AUTO = 7; // Number of autos

// Auto names
  const char *autoNames[NUM_AUTO] = {
    "Left", 
    "Saucin'",
    "Right",
    "Right Claw",
    "WP",
    "Skills",
    "Test"
  };

void decAuto() {
  currAuto--;
  cycleNum();
  Brain.Screen.clearScreen();
  Brain.Screen.printAt(24, 48, autoNames[currAuto]);
}

void incAuto() {
  currAuto++;
  cycleNum();
  Brain.Screen.clearScreen();
  Brain.Screen.printAt(24, 48, autoNames[currAuto]);
}

void cycleNum() {
  // Make sure the auto number isnt too low or high
  if (currAuto < 0)
    currAuto = MAX_INDEX;

  if (currAuto > MAX_INDEX)
    currAuto = 0;
}

void autoSelector() {
  Brain.Screen.clearScreen();
  Brain.Screen.printAt(24, 48, autoNames[currAuto]);

  LAutoClick.released(decAuto);
  RAutoClick.released(incAuto);
}