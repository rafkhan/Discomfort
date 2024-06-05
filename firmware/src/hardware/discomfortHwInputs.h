#ifndef DISCOMFORT_HW_INPUTS_H
#define DISCOMFORT_HW_INPUTS_H

#include "daisy_patch_sm.h"
#include "pins.h"
#include "mux.h"
#include "../DiscomfortInput.h"

using namespace daisy;
using namespace patch_sm;

// float GetMuxFloat(int muxId, int pin);

float getScaledPotInput(float in);
float getScaledCvInput(float in);
// DiscomfortHwInputs getInputsFromHw(DaisyPatchSM *hw, Mux **muxes);


class DiscomfortHwAnalogInput
{
public:
  DiscomfortHwAnalogInput(DaisyPatchSM *_hw, Mux *_mux, int _muxPin, int _muxIdx);
  float raw;
  float value;
  float getValue();
  void read(void);

  int muxIdx;
  int muxPin;
private:
  Mux *mux;
  DaisyPatchSM *hw;
};


class DiscomfortHwInputs
{
public:
  DiscomfortHwInputs(DaisyPatchSM *hw, Mux **mux);
  DiscomfortInput createDiscomfortInput(float audioIn);
  void updateAll(void);

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

  DiscomfortHwAnalogInput *foldAmountCv;
  DiscomfortHwAnalogInput *foldAmountPot;

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

#endif