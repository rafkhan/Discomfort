#include "daisy_patch_sm.h"
#include "daisysp.h"

#include "src/Discomfort.h"
#include "src/util.h"
#include "src/Clipper.h"
#include "src/Folder.h"
#include "src/FilterBank.h"
#include "src/DryWet.h"

#include "src/hardware/mux.h"
#include "src/hardware/discomfortHwInputs.h"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

float sampleRate;
DaisyPatchSM hw;

Discomfort distChannelL;
Discomfort distChannelR;

// Can move into mux class probably
dsy_gpio muxSelect0;
dsy_gpio muxSelect1;
dsy_gpio muxSelect2;

Mux mux0;
Mux mux1;
Mux mux2;
Mux *muxes[3] = {&mux0, &mux1, &mux2};
DiscomfortHwInputs *hardwareInputs;
int muxPinIdx = 0; // for iterating over the all mux select pins

void initAdc()
{
  mux0.init(&muxSelect0, &muxSelect1, &muxSelect2, &hw, CV_8);
  mux1.init(&muxSelect0, &muxSelect1, &muxSelect2, &hw, CV_7);
  mux2.init(&muxSelect0, &muxSelect1, &muxSelect2, &hw, CV_6);
}

DiscomfortOutput process(float audioIn, DiscomfortHwInputs *hwInputs, Discomfort *ch)
{
  DiscomfortInput inputStruct = hardwareInputs->createDiscomfortInput(audioIn);
  return ch->process(inputStruct);
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
  for (size_t i = 0; i < size; i++)
  {
    DiscomfortOutput outputL = process(IN_L[i], hardwareInputs, &distChannelL);
    DiscomfortOutput outputR = process(IN_R[i], hardwareInputs, &distChannelR);

    OUT_L[i] = outputL.audioOutput;
    OUT_R[i] = outputR.audioOutput;

    // // hw.WriteCvOut(1, 5.f * outputStructL.followerOutput);
    // // hw.WriteCvOut(2, 5.f * outputStructL.followerOutput);
  }
}

void printThing(DiscomfortHwAnalogInput *x) {
    hw.Print(
      "(%d, %d): %f\t",
      x->muxIdx,
      x->muxPin,
      x->getValue()
    );
}

int main(void)
{
  hw.Init();
  hw.StartLog();

  System::Delay(100);

  hw.SetAudioBlockSize(16);
  hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
  sampleRate = hw.AudioSampleRate();

  initAdc();

  distChannelL.init(sampleRate);
  distChannelR.init(sampleRate);

  hw.StartAudio(AudioCallback);
  Mux *muxes[3] = {&mux0, &mux1, &mux2};
  hardwareInputs = new DiscomfortHwInputs(&hw, muxes);

  while (1)
  {
    // this should probably block and fuck things up???
    // inputs = getInputsFromHw(&hw, muxes);
    if (muxPinIdx % 8 == 0)
    {
      muxPinIdx = 0;
    }
    hardwareInputs->readMuxOnePin(&hw, muxPinIdx);
    muxPinIdx++;

    hardwareInputs->distEnvAttenuverter->setValue(hw.GetAdcValue(CV_1));
    hardwareInputs->foldEnvAttenuverter->setValue(hw.GetAdcValue(CV_2));
    System::Delay(1);

    // hw.PrintLine();

    // hardwareInputs->updateAll();
    // hw.PrintLine(
    //     "(%d, %d): %f",
    //     hardwareInputs->foldAmountPot->muxIdx,
    //     hardwareInputs->foldAmountPot->muxPin,
    //     fclamp(getScaledPotInput(hardwareInputs->foldAmountPot->getValue()) + getScaledCvInput(hardwareInputs->foldAmountCv->getValue()), 0, 1));
    // printThing(hardwareInputs->distParamCPot);
    // printThing(hardwareInputs->distParamCCv);
    // printThing(hardwareInputs->distParamBPot);
    // printThing(hardwareInputs->distParamBCv);
    // printThing(hardwareInputs->foldSymmetryPot);
    // printThing(hardwareInputs->foldEnvAttenuverter);
    // printThing(hardwareInputs->foldEnvAttenuverter);
    // hw.PrintLine("");
  }
}
