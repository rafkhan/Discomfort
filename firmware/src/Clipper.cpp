#include <cmath>
#include "Clipper.h"

float Clipper::clip(float input, float gain, ClipperType type) {
  return tanh(input * gain);
}