#include "DiscomfortHwInputs.h"
#include "../DiscomfortInput.h"

// float GetMuxFloat(Mux *mux[], DaisyPatchSM *hw, int muxId, int pin)
// {
//   System::Delay(1);
//   return mux[muxId]->getInput(pin, true, hw);
// }


float getScaledPotInput(float in)
{
  return map(0.5 - in, 0, 0.5, 0, 1);
}


DiscomfortHwInputs::DiscomfortHwInputs(DaisyPatchSM *hw, Mux **muxes) {
  this->foldAmountPot = new DiscomfortHwAnalogInput(hw, muxes[0], 3, 0);
  this->foldAmountCv = new DiscomfortHwAnalogInput(hw, muxes[0], 5, 0);
}

void DiscomfortHwInputs::updateAll(void) {
  this->foldAmountPot->read();
}

DiscomfortInput DiscomfortHwInputs::createDiscomfortInput(float audioIn) {
  DiscomfortInput inputs = DiscomfortInput::create();
  inputs.input = audioIn;
  inputs.setFolderValues(
    map(getScaledPotInput(this->foldAmountPot->getValue()), 0.f, 1.f, FOLDER_MIN_GAIN, FOLDER_MAX_GAIN),
    0,
    0
  );

  return inputs;
}

// DiscomfortHwInputs getInputsFromHw(DaisyPatchSM *hw, Mux **mux)
// {
//   inputs.distEnvAttenuverter = hw->GetAdcValue(CV_1);
//   inputs.foldEnvAttenuverter = hw->GetAdcValue(CV_2);

//   inputs.envGain = GetMuxFloat(mux, hw, 0, 7);
//   inputs.envDecay = GetMuxFloat(mux, hw, 0, 6);
//   inputs.envAttack = GetMuxFloat(mux, hw, 1, 1);

//   inputs.foldAmountCv = GetMuxFloat(mux, hw, 0, 5);
//   inputs.foldOffsetCv = GetMuxFloat(mux, hw, 0, 4);

//   // inputs.foldAmountPot = GetMuxFloat(mux, hw, 0, 3);

//   inputs.foldOffsetPot = GetMuxFloat(mux, hw, 0, 2);
//   inputs.foldSymmetryCv = GetMuxFloat(mux, hw, 0, 1);
//   inputs.foldSymmetryPot = GetMuxFloat(mux, hw, 0, 0);

//   inputs.distParamBCv = GetMuxFloat(mux, hw, 1, 7);
//   inputs.distParamAPot = GetMuxFloat(mux, hw, 1, 6);
//   inputs.distParamBPot = GetMuxFloat(mux, hw, 1, 5);
//   inputs.distParamACv = GetMuxFloat(mux, hw, 1, 4);
//   inputs.distParamCCv = GetMuxFloat(mux, hw, 1, 3);
//   inputs.distParamCPot = GetMuxFloat(mux, hw, 1, 0);

//   inputs.distMixCv = GetMuxFloat(mux, hw, 2, 7);
//   inputs.foldMixPot = GetMuxFloat(mux, hw, 2, 6);
//   inputs.distMixPot = GetMuxFloat(mux, hw, 2, 5);
//   inputs.foldMixCv = GetMuxFloat(mux, hw, 2, 4);
//   inputs.outputGain = GetMuxFloat(mux, hw, 2, 3);
//   inputs.inputGain = GetMuxFloat(mux, hw, 2, 2);

//   return inputs;
// }

DiscomfortHwAnalogInput::DiscomfortHwAnalogInput(DaisyPatchSM *_hw, Mux *_mux, int _muxPin, int _muxIdx) {
  muxIdx = _muxIdx;
  muxPin = _muxPin;
  mux = _mux;
  hw = _hw;
  raw = 0.f;
  value = 0.f;
}

float DiscomfortHwAnalogInput::getValue() {
  return this->value;
}

void DiscomfortHwAnalogInput::read(void) {
  System::Delay(1);
  this->value = mux->getInput(muxPin, true, hw);
}