#include "daisy_patch_sm.h"
#include "daisysp.h"

#include "src/DiscomfortInternal/Discomfort.h"
#include "src/DiscomfortInternal/util.h"
#include "src/DiscomfortInternal/Clipper.h"
#include "src/DiscomfortInternal/Folder.h"
#include "src/DiscomfortInternal/FilterBank.h"
#include "src/DiscomfortInternal/DryWet.h"

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

float asd[4] = { 0, 0, 0, 0 };

void initAdc()
{
  mux0.init(&muxSelect0, &muxSelect1, &muxSelect2, &hw, 0);
  mux1.init(&muxSelect0, &muxSelect1, &muxSelect2, &hw, 1);
  mux2.init(&muxSelect0, &muxSelect1, &muxSelect2, &hw, 2);
}

DiscomfortOutput process(float audioIn, DiscomfortHwInputs *hwInputs, Discomfort *ch)
{
  DiscomfortInput inputStruct = hardwareInputs->createDiscomfortInput(audioIn);
  return ch->process(inputStruct);
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
  // hardwareInputs->readMuxOnePin(&hw, muxPinIdx);

  for(int i = 0; i < 4; i++) {
    // muxes[0]->setBits(i);
    // dsy_gpio_write(&muxSelect0, (bool) ((i >> 0) & 1));
    // dsy_gpio_write(&muxSelect0, (bool) ((i >> 1) & 1));
    // dsy_gpio_write(&muxSelect0, (bool) ((i >> 2) & 1));
    asd[i] = hw.adc.GetFloat(0);

    // DiscomfortHwAnalogInput *input = this->readMapper[i][pin];
    // if(input != nullptr) {
    //   // input->read(false);
    //   input->setValue(hw->adc.GetFloat(0));
    // }
    // System::Delay(1);
    // hw->Print("%d, %f \t", i, hw->adc.GetFloat(0));
  }


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
    hw.PrintLine(
      "(%d, %d): %f\t",
      x->muxIdx,
      x->muxPin,
      x->getValue()
    );
}

int main(void)
{
  hw.Init();

  hw.adc.Stop();

  AdcChannelConfig adc[3];
  adc[0].InitSingle(hw.C7);
  adc[1].InitSingle(hw.C6);
  adc[2].InitSingle(hw.C5);
  // adc[3].InitSingle(hw.C6);
  // adc[4].InitSingle(hw.C5);
  // adc[5].InitSingle(hw.C4);
  // adc[6].InitSingle(hw.C3);
  // adc[7].InitSingle(hw.C2);
  // adc[8].InitSingle(hw.C1);

  hw.adc.Init(adc, 3);
  hw.adc.Start();

  initAdc();

  hw.StartLog();
  System::Delay(100);

  hw.SetAudioBlockSize(8);
  hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
  sampleRate = hw.AudioSampleRate();

  distChannelL.init(sampleRate);
  distChannelR.init(sampleRate);

  hw.StartAudio(AudioCallback);
  Mux *muxes[3] = {&mux0, &mux1, &mux2};
  hardwareInputs = new DiscomfortHwInputs(&hw, muxes);

  dsy_gpio_write(&muxSelect0, (bool) ((3 >> 0) & 1));
  dsy_gpio_write(&muxSelect0, (bool) ((3 >> 1) & 1));
  dsy_gpio_write(&muxSelect0, (bool) ((3 >> 2) & 1));

  while (1)
  {
    for (int i = 0; i < 4; i++) {
      hw.PrintLine("asd[%d]: %f", i, asd[i]);
    }
    hw.PrintLine("");

    // this should probably block and fuck things up???
    // inputs = getInputsFromHw(&hw, muxes);
    // if (muxPinIdx % 8 == 0)
    // {
    //   muxPinIdx = 0;
    // }
    // muxPinIdx++;

    // hardwareInputs->distEnvAttenuverter->setValue(hw.GetAdcValue(CV_1));
    // hardwareInputs->foldEnvAttenuverter->setValue(hw.GetAdcValue(CV_2));
    // System::Delay(1);
    // printThing(hardwareInputs->foldAmountPot);

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
