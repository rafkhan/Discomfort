#include <cmath>
#include "Shaper.h"

float Shaper::shape(float input, float gain) {
  return tanh(input * gain);
}
