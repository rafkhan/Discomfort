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
// DiscomfortHwInputs inputs;
DiscomfortHwInputs *hardwareInputs;

void initAdc()
{
  mux0.init(&muxSelect0, &muxSelect1, &muxSelect2, &hw, CV_8);
  mux1.init(&muxSelect0, &muxSelect1, &muxSelect2, &hw, CV_7);
  mux2.init(&muxSelect0, &muxSelect1, &muxSelect2, &hw, CV_6);
}

// DiscomfortOutput process(float audioIn, DiscomfortHwInputs *hwInputs, Discomfort *ch)
// {
//   // optimize this please
//   // DiscomfortInput inputStruct = hardwareInputs->createDiscomfortInput(audioIn);
//   // // DiscomfortInput dcInput = DiscomfortInput::create();
//   // // dcInput.setFolderValues(20, 0, 0);
//   // // dcInput.input = audioIn;
//   // return ch->process(inputStruct);
//   return nullptr;
// }

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
  for (size_t i = 0; i < size; i++)
  {
    // DiscomfortOutput outputL = process(IN_L[i], hardwareInputs, &distChannelL);
    // DiscomfortOutput outputR = process(IN_R[i], hardwareInputs, &distChannelR);
    // OUT_L[i] = outputL.audioOutput;
    // OUT_R[i] = outputR.audioOutput;

    // OUT_L[i] = Folder::fold(IN_L[i], map(inputs.foldAmountPot, 0, 1, FOLDER_MIN_GAIN, FOLDER_MAX_GAIN), 0, 0);
    // OUT_R[i] = 0;

    // // hw.WriteCvOut(1, 5.f * outputStructL.followerOutput);
    // // hw.WriteCvOut(2, 5.f * outputStructL.followerOutput);
    OUT_L[i] = IN_L[i];
    OUT_R[i] = IN_R[i];
  }
}

int main(void)
{
  hw.Init();
  hw.StartLog();

  System::Delay(100);

  hw.SetAudioBlockSize(4);
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
    hardwareInputs->updateAll();
    hw.PrintLine(
      "(%d, %d): %f",
      hardwareInputs->foldAmountPot->muxIdx,
      hardwareInputs->foldAmountPot->muxPin,
      hardwareInputs->foldAmountPot->getValue()
    );
  }
}
