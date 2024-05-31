#ifndef OKAYMUX_H
#define OKAYMUX_H

#include "daisy_patch_sm.h"
#include "daisysp.h"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

class Mux {
  public:
    void init(dsy_gpio *s0, dsy_gpio *s1, dsy_gpio *s2, DaisyPatchSM *hw, int _cvIdx);
    float getInput(uint8_t pin, bool writeSelectPin, DaisyPatchSM *hw);

  private:
    dsy_gpio *s0, *s1, *s2;
    DaisyPatchSM *hw;
    int cvIdx;
    void setPinCfg(dsy_gpio *gpio, Pin pin);
};

#endif