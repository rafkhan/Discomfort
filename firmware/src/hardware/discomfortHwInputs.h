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
  void setValue(float _value);
  void read(bool writePins);
  void setMuxBits(int pin);

  int muxIdx;
  int muxPin;

private:
  Mux *mux;
  DaisyPatchSM *hw;
};

class DiscomfortHwInputs
{
public:
  DiscomfortHwInputs(DaisyPatchSM *hw, Mux **_muxes);
  DiscomfortInput createDiscomfortInput(float audioIn);
  void updateAll(void);
  void readMuxOnePin(DaisyPatchSM *hw, int pin);

  Mux **muxes;

  DiscomfortHwAnalogInput *inputGain;
  DiscomfortHwAnalogInput *outputGain;

  DiscomfortHwAnalogInput *envGain;
  DiscomfortHwAnalogInput *envAttack;
  DiscomfortHwAnalogInput *envDecay;

  DiscomfortHwAnalogInput *foldMixCv;
  DiscomfortHwAnalogInput *foldMixPot;
  DiscomfortHwAnalogInput *foldOffsetCv;
  DiscomfortHwAnalogInput *foldOffsetPot;
  DiscomfortHwAnalogInput *foldSymmetryCv;
  DiscomfortHwAnalogInput *foldSymmetryPot;

  DiscomfortHwAnalogInput *foldAmountCv;
  DiscomfortHwAnalogInput *foldAmountPot;

  DiscomfortHwAnalogInput *foldEnvAttenuverter;

  DiscomfortHwAnalogInput *distMixCv;
  DiscomfortHwAnalogInput *distMixPot;
  DiscomfortHwAnalogInput *distParamACv;
  DiscomfortHwAnalogInput *distParamAPot;
  DiscomfortHwAnalogInput *distParamBCv;
  DiscomfortHwAnalogInput *distParamBPot;
  DiscomfortHwAnalogInput *distParamCCv;
  DiscomfortHwAnalogInput *distParamCPot;
  DiscomfortHwAnalogInput *distEnvAttenuverter;

private:
  // convenience code for me as I develop lol
  DiscomfortHwAnalogInput *readMapper[3][8];
  DiscomfortHwAnalogInput* createAnalogHwInput(DaisyPatchSM *hw, int mux, int pin);
};

#endif