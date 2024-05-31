#ifndef PINS_H
#define PINS_H

#include "daisy_patch_sm.h"

using namespace daisy;
using namespace patch_sm;

struct DiscomfortPins {
  dsy_gpio_pin MUXS0;
  dsy_gpio_pin MUXS1;
  dsy_gpio_pin MUXS2;
  dsy_gpio_pin MUXACD0;
  dsy_gpio_pin MUXACD1;
  dsy_gpio_pin MUXACD2;
};

DiscomfortPins getPins();

#endif