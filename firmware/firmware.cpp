#include "daisy_patch_sm.h"
#include "daisysp.h"

#include "src/Discomfort.h"
#include "src/util.h"
#include "src/Clipper.h"
#include "src/Folder.h"
#include "src/FilterBank.h"

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

void initAdc()
{
  mux0.init(&muxSelect0, &muxSelect1, &muxSelect2, &hw, CV_8);
  mux1.init(&muxSelect0, &muxSelect1, &muxSelect2, &hw, CV_7);
  mux2.init(&muxSelect0, &muxSelect1, &muxSelect2, &hw, CV_6);
}

DiscomfortOutput process(float audioIn, DiscomfortHwInputs hwInputs, Discomfort *ch)
{
  DiscomfortInput inputStruct = DiscomfortInput::createFromHwInputs(audioIn, hwInputs);
  return ch->process(inputStruct);
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
  // hw.ProcessAllControls();
  DiscomfortHwInputs inputs = getInputsFromHw(&hw, muxes);

  for (size_t i = 0; i < size; i++)
  {
    DiscomfortOutput outputL = process(IN_L[i], inputs, &distChannelL);
    DiscomfortOutput outputR = process(IN_L[i], inputs, &distChannelL);
    // hw.WriteCvOut(1, 5.f * outputStructL.followerOutput);
    // hw.WriteCvOut(2, 5.f * outputStructL.followerOutput);
    OUT_L[i] = outputL.audioOutput;
    OUT_R[i] = outputR.audioOutput;
  }
}

int main(void)
{
  hw.Init();
  hw.StartLog();

  System::Delay(100);

  hw.SetAudioBlockSize(8);
  hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_96KHZ);
  sampleRate = hw.AudioSampleRate();

  initAdc();

  distChannelL.init(sampleRate);
  distChannelR.init(sampleRate);

  hw.StartAudio(AudioCallback);

  while (1)
  {
  }
}
