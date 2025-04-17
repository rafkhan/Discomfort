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

float a = 0;

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
  for (size_t i = 0; i < size; i++)
  {
    OUT_L[i] = IN_L[i];
    OUT_R[i] = IN_R[i];
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
    if (muxPinIdx % 8 == 0)
    {
      muxPinIdx = 0;
    }
    muxPinIdx++;

    // Set select pins
    dsy_gpio_write(&muxSelect0, (bool) ((muxPinIdx >> 0) & 1));
    dsy_gpio_write(&muxSelect1, (bool) ((muxPinIdx >> 1) & 1));
    dsy_gpio_write(&muxSelect2, (bool) ((muxPinIdx >> 2) & 1));

    if(muxPinIdx == 0) {
      a = hw.adc.GetFloat(0);
    }
    float b = hw.adc.GetFloat(1);
    float c = hw.adc.GetFloat(2);
  }
}
