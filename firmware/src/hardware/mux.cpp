#include "daisy_patch_sm.h"
#include "daisysp.h"
#include "mux.h"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

void Mux::init(dsy_gpio *s0, dsy_gpio *s1, dsy_gpio *s2, DaisyPatchSM *hw, int _cvIdx) {
  this->s0 = s0;
  this->s1 = s1;
  this->s2 = s2;
  this->hw = hw;
  this->cvIdx = _cvIdx;

  setPinCfg(this->s0, DaisyPatchSM::D5);
  setPinCfg(this->s1, DaisyPatchSM::D4);
  setPinCfg(this->s2, DaisyPatchSM::D3);
}

void Mux::setPinCfg(dsy_gpio *gpio, Pin pin) {
 	gpio->pin = pin;
	gpio->mode = DSY_GPIO_MODE_OUTPUT_PP;
	gpio->pull = DSY_GPIO_NOPULL;
	dsy_gpio_init(gpio); 
}

float Mux::getInput(int pin, bool writeSelectPins, DaisyPatchSM *hw) {
  if(writeSelectPins) {
    dsy_gpio_write(s0, (bool) ((pin >> 0) & 1));
    dsy_gpio_write(s1, (bool) ((pin >> 1) & 1));
    dsy_gpio_write(s2, (bool) ((pin >> 2) & 1));
  }

  hw->ProcessAnalogControls();
  return hw->GetAdcValue(this->cvIdx);
}

void Mux::setBits(int pin) {
  dsy_gpio_write(s0, (bool) ((pin >> 0) & 1));
  dsy_gpio_write(s1, (bool) ((pin >> 1) & 1));
  dsy_gpio_write(s2, (bool) ((pin >> 2) & 1));
}