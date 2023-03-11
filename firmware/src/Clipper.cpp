#include <cmath>
#include "Clipper.h"
#include "util.h"

float Clipper::clip(float input, float gain, ClipperType type) {
  float y = input * gain;
  if (type == CLIPPER_SOFT) {
    return tanh(y);
  }

  if (type == CLIPPER_ALT) {
    return sin(y);
  }

  // Hard clip
  if (y > 1.0f) {
    return 1.0f;
  }

  if (y < -1.0f) {
    return -1.0f;
  }

  return y;
}