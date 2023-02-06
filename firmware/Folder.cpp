#include <cmath>
#include "Folder.h"

float Folder::fold(float input, float gain) {
  // Only concern ourselves with positive domain for now
  if(input <= 0) {
    return input;
  }

  // input * gain does not exceed folding threshold
  float newInput = input * gain;
  if(newInput <= 1) {
    return newInput;
  }

  int numberOfFolds = (int) floor(newInput / 2);
  
  if(numberOfFolds % 2 == 0) {
    return -1 + (newInput - numberOfFolds * 2.0f);
  } else {
    return 1 - (newInput - numberOfFolds * 2.0f);
  }
}