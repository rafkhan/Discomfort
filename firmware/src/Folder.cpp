#include <cmath>
#include "Folder.h"

float Folder::fold(float input, float gain, float offset) {
  float sign = 1;
  if(input < 0) {
    sign = -1;
    input = input * -1;
  }

  float newInput = input * gain;

  // input * gain does not exceed folding threshold
  // no need to compute anything further 
  if (newInput <= 1) {
    return newInput * sign;
  }

  int numberOfFolds = (int) floor((newInput + 1) / 2);
  numberOfFolds = numberOfFolds == 0 ? 1 : numberOfFolds;
  
  float foldValue = 0;
  float sub = numberOfFolds == 1 ? numberOfFolds : (numberOfFolds * 2.0f - 1);
  if(numberOfFolds % 2 == 0) {
    foldValue = -1 + (newInput - sub);
  } else {
    foldValue = 1 - (newInput - sub);
  }

  return foldValue * sign;
}