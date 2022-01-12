#include "vex.h"

bool mogoClamped = false;

// Auto
void mogoAuto(bool isClamping) {
  mogoClamp.set(isClamping);
  mogoClamped = isClamping;
}

int mogoLiftAsync() {
  while (1) {
    task::sleep(20); // Sleep to save resources
  }

  return 0;
}

// Utility
void mogoToggle() {
  mogoClamp.set(!mogoClamped);
  mogoClamped = !mogoClamped;
}
