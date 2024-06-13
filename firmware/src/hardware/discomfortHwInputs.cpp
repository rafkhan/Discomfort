#include "DiscomfortHwInputs.h"
#include "../DiscomfortInput.h"

float getScaledPotInput(float in)
{
  return map(0.5 - in, 0, 0.5, 0, 1);
}

float getScaledCvInput(float in)
{
  return map(1.f - in, 0, 1, -1, 1);
}

DiscomfortHwInputs::DiscomfortHwInputs(DaisyPatchSM *hw, Mux **_muxes)
{
  this->muxes = _muxes;

  for (int a = 0; a < 8; a++)
  {
    for (int b = 0; b < 8; b++)
    {
      readMapper[a][b] = nullptr;
    }
  }

  this->foldSymmetryPot = createAnalogHwInput(hw, 0, 0);
  this->foldSymmetryCv = createAnalogHwInput(hw, 0, 1);
  this->foldAmountPot = createAnalogHwInput(hw, 0, 3);
  this->foldAmountCv = createAnalogHwInput(hw, 0, 5);

  this->distParamCPot = createAnalogHwInput(hw, 1, 0);
  this->distParamBCv = createAnalogHwInput(hw, 1, 3);
  this->distParamBPot = createAnalogHwInput(hw, 1, 5);
  this->distParamCCv = createAnalogHwInput(hw, 1, 7);

  this->foldEnvAttenuverter = createAnalogHwInput(hw, 4, 10);
  this->distEnvAttenuverter = createAnalogHwInput(hw, 4, 10);
}

DiscomfortHwAnalogInput *DiscomfortHwInputs::createAnalogHwInput(DaisyPatchSM *hw, int mux, int pin)
{
  DiscomfortHwAnalogInput *i = new DiscomfortHwAnalogInput(hw, this->muxes[mux], pin, mux);
  this->readMapper[mux][pin] = i;
  return i;
}

void DiscomfortHwInputs::updateAll(void) {}

void DiscomfortHwInputs::readMuxOnePin(DaisyPatchSM *hw, int pin)
{
  this->muxes[0]->setBits(pin);
  System::Delay(1);

  for (int i = 0; i < 3; i++)
  {
    DiscomfortHwAnalogInput *input = this->readMapper[i][pin];
    if (input != nullptr)
    {
      input->read(false);
    }
  }

  this->distEnvAttenuverter->setValue(hw->GetAdcValue(CV_1));
  this->foldEnvAttenuverter->setValue(hw->GetAdcValue(CV_2));
  // hw->PrintLine("asd %f", this->distEnvAttenuverter->getValue());

  // THIS IS BRAIN DEAD CODE

  // if (pin == 3)
  // {
  //   this->foldAmountPot->read(false);
  // }

  // if (pin == 5)
  // {
  //   this->foldAmountCv->read(false);
  // }

  // if (pin == 0)
  // {
  //   this->foldSymmetryPot->read(false);
  //   this->distParamCPot->read(false);
  // }
}

DiscomfortInput DiscomfortHwInputs::createDiscomfortInput(float audioIn)
{
  DiscomfortInput inputs = DiscomfortInput::create();
  inputs.input = audioIn;

  inputs.setFolderValues(
      fclamp(getScaledPotInput(this->foldAmountPot->getValue()) + getScaledCvInput(this->foldAmountCv->getValue()), 0, 1),
      0,
      0,
      getScaledPotInput(this->foldSymmetryPot->getValue())); // temporary mix knob

  inputs.setClipperValues(
      getScaledPotInput(this->distParamCPot->getValue()),
      0,
      getScaledPotInput(this->distParamBPot->getValue()));

  return inputs;
}

DiscomfortHwAnalogInput::DiscomfortHwAnalogInput(DaisyPatchSM *_hw, Mux *_mux, int _muxPin, int _muxIdx)
{
  muxIdx = _muxIdx;
  muxPin = _muxPin;
  mux = _mux;
  hw = _hw;
  raw = 0.f;
  value = 0.f;
}

void DiscomfortHwAnalogInput::setValue(float _value)
{
  this->value = value;
}

float DiscomfortHwAnalogInput::getValue()
{
  return this->value;
}

void DiscomfortHwAnalogInput::read(bool writePins)
{
  this->value = mux->getInput(muxPin, writePins, hw);
}

void DiscomfortHwAnalogInput::setMuxBits(int pin)
{
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