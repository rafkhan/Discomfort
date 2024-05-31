#include "discomfortHwInputs.h"

float GetMuxFloat(Mux *mux[], DaisyPatchSM *hw, int muxId, int pin) {
  System::Delay(1);
  return mux[muxId]->getInput(pin, true, hw);
}

DiscomfortHwInputs getInputsFromHw(DaisyPatchSM *hw, Mux **mux) {
  DiscomfortHwInputs inputs;

  inputs.audioIn = 0.f;

  inputs.distEnvAttenuverter = hw->GetAdcValue(CV_1);
  inputs.foldEnvAttenuverter = hw->GetAdcValue(CV_2);

  inputs.envGain = GetMuxFloat(mux, hw, 0, 7);
  inputs.envDecay = GetMuxFloat(mux, hw, 0, 6);
  inputs.envAttack = GetMuxFloat(mux, hw, 1, 1);

  inputs.foldAmountCv = GetMuxFloat(mux, hw, 0, 5); 
  inputs.foldOffsetCv = GetMuxFloat(mux, hw, 0, 4); 

  inputs.foldAmountPot = GetMuxFloat(mux, hw, 0, 3); 

  inputs.foldOffsetPot = GetMuxFloat(mux, hw, 0, 2); 
  inputs.foldSymmetryCv = GetMuxFloat(mux, hw, 0, 1); 
  inputs.foldSymmetryPot = GetMuxFloat(mux, hw, 0, 0); 

  inputs.distParamBCv = GetMuxFloat(mux, hw, 1, 7);
  inputs.distParamAPot = GetMuxFloat(mux, hw, 1, 6);
  inputs.distParamBPot = GetMuxFloat(mux, hw, 1, 5);
  inputs.distParamACv = GetMuxFloat(mux, hw, 1, 4);
  inputs.distParamCCv = GetMuxFloat(mux, hw, 1, 3);
  inputs.distParamCPot = GetMuxFloat(mux, hw, 1, 0);

  // inputs.distMixCv = GetMuxFloat(mux, hw, 2, 7);
  // inputs.foldMixPot = GetMuxFloat(mux, hw, 2, 6);
  // inputs.distMixPot = GetMuxFloat(mux, hw, 2, 5);
  // inputs.foldMixCv = GetMuxFloat(mux, hw, 2, 4);
  // inputs.outputGain = GetMuxFloat(mux, hw, 2, 3);
  // inputs.inputGain = GetMuxFloat(mux, hw, 2, 2);

  return inputs;
}
