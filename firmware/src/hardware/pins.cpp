#include "daisy_patch_sm.h"

#include "pins.h"

using namespace daisy;
using namespace patch_sm;

DiscomfortPins getPins() {
  DiscomfortPins pins;
  pins.MUXS0 = DaisyPatchSM::D5;
  pins.MUXS1 = DaisyPatchSM::D4;
  pins.MUXS2 = DaisyPatchSM::D3;
  pins.MUXACD0 = DaisyPatchSM::C9;
  pins.MUXACD1 = DaisyPatchSM::C8;
  pins.MUXACD2 = DaisyPatchSM::C7; // NOT CONNECTED IN HARDWARE :(

  return pins;
}