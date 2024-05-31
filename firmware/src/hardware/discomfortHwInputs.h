#ifndef DISCOMFORT_INPUTS_H 
#define DISCOMFORT_INPUTS_H

#include "daisy_patch_sm.h"
#include "pins.h"
#include "mux.h"
#include "../DiscomfortInput.h"

using namespace daisy;
using namespace patch_sm;

struct DiscomfortHwInputs {
  float audioIn;

  float inputGain;
  float outputGain;

  float envGain;
  float envAttack;
  float envDecay;

  float foldMixCv;
  float foldMixPot;
  float foldOffsetCv;
  float foldOffsetPot;
  float foldSymmetryCv;
  float foldSymmetryPot;
  float foldAmountCv;
  float foldAmountPot;
  float foldEnvAttenuverter;

  float distMixCv;
  float distMixPot;
  float distParamACv;
  float distParamAPot;
  float distParamBCv;
  float distParamBPot;
  float distParamCCv;
  float distParamCPot;
  float distEnvAttenuverter;
};

float GetMuxFloat(int muxId, int pin);
DiscomfortHwInputs getInputsFromHw(DaisyPatchSM *hw, Mux **mux);
DiscomfortInput create();


#endif