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

dsy_gpio btn1Pin;

Mux mux0;
Mux mux1;
Mux mux2;
Mux *muxes[3] = {&mux0, &mux1, &mux2};
DiscomfortHwInputs *hardwareInputs;
int muxPinIdx = 0; // for iterating over the all mux select pins

Switch button1;
Switch button2;

DiscomfortOutput *outputL;
DiscomfortOutput *outputR;

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
    DiscomfortOutput newL = process(IN_L[i], hardwareInputs, &distChannelL);
    DiscomfortOutput newR = process(IN_R[i], hardwareInputs, &distChannelR);

    outputL->audioOutput = newL.audioOutput;
    outputL->followerOutput = newL.followerOutput;
    outputR->audioOutput = newL.audioOutput;
    outputR->followerOutput = newL.followerOutput;

    OUT_L[i] = outputL->audioOutput;
    OUT_R[i] = outputR->audioOutput;

    hw.WriteCvOut(1, 5.f * outputL->followerOutput);
    hw.WriteCvOut(2, 5.f * outputL->followerOutput);
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
  hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_96KHZ);
  sampleRate = hw.AudioSampleRate();

  initAdc();

  distChannelL.init(sampleRate);
  distChannelR.init(sampleRate);

  // LED
  dsy_gpio gpio;
 	gpio.pin = DaisyPatchSM::B7;
	gpio.mode = DSY_GPIO_MODE_OUTPUT_PP;
	gpio.pull = DSY_GPIO_NOPULL;
	dsy_gpio_init(&gpio); 

  button1.Init(DaisyPatchSM::B8, 1000);
  button2.Init(DaisyPatchSM::C6, 1000);

  Mux *muxes[3] = {&mux0, &mux1, &mux2};
  hardwareInputs = new DiscomfortHwInputs(&hw, muxes);

  hw.StartAudio(AudioCallback);

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

    button1.Debounce();
    button2.Debounce();
    bool b1 = button1.Pressed();
    bool b2 = button2.Pressed();

    dsy_gpio_write(&gpio, b1);

    hw.PrintLine("%d %d", b1, b2);

    hardwareInputs->distEnvAttenuverter->setValue(hw.GetAdcValue(CV_1));
    hardwareInputs->foldEnvAttenuverter->setValue(hw.GetAdcValue(CV_2));

    // System::Delay(1); // IS THIS MAYBE EXTRA SLEEP??????
    // printThing(hardwareInputs->foldEnvAttenuverter);
  }
}
