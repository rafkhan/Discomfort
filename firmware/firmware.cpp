#include "daisy_patch_sm.h"
#include "daisysp.h"

#include "src/Discomfort.h"
#include "src/util.h"
#include "src/Clipper.h"
#include "src/Folder.h"
#include "src/FilterBank.h"
#include "src/PageBank/PageBank.h"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

Switch button;

float sampleRate;
DaisyPatchSM hw;

Discomfort distChannelL;
Discomfort distChannelR;

PageBank pageEnvFollower;
PageBank pageFolder;
PageBank pageClipper;

// temp shit
Oscillator osc;

float pot4 = 0;
float cv1 = 0;
float cv2 = 0;
float cv3 = 0;
float cv4 = 0;


PageBank *banks[] = { &pageFolder, &pageClipper };
int buttonState = 0;
int maxButton = 1;
void onButtonPress() {
  banks[buttonState]->lockPage();
  buttonState++;
  if (buttonState > maxButton) {
    buttonState = 0;
  }
}

void readAllAdcInputs()
{
  pot4 = hw.GetAdcValue(CV_4);
  cv1 = hw.GetAdcValue(CV_5);
  cv2 = hw.GetAdcValue(CV_6);
  cv3 = hw.GetAdcValue(CV_7);
  cv4 = hw.GetAdcValue(CV_8);

  // hw.WriteCvOut(2, 5.f * buttonState);

  banks[buttonState]->updatePage(
    hw.GetAdcValue(CV_1),
    hw.GetAdcValue(CV_2),
    hw.GetAdcValue(CV_3),
    pot4,
    cv1,
    cv2,
    cv3,
    cv4
  );
}

DiscomfortOutput process(float audioIn, Discomfort *ch) {
  DiscomfortInput inputStruct = DiscomfortInput::create(audioIn);

  inputStruct.setFolderValues(
    fclamp(pageFolder.pot1.getValue() + cv1, 0, 1),
    fclamp(pageFolder.pot2.getValue() + cv2, -1, 1),
    fclamp(pageFolder.pot3.getValue() + cv3, -1, 1)
  );

  inputStruct.setClipperValues(
    fclamp(pageClipper.pot1.getValue(), 0, 1),
    fclamp(pageClipper.pot2.getValue(), 0, 1)
  );

  inputStruct.dryWet = pot4;

  return ch->process(inputStruct);
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
  hw.ProcessAllControls();
  button.Debounce();

  if (button.RisingEdge()) {
      onButtonPress();
  }

  for (size_t i = 0; i < size; i++)
  {
    DiscomfortOutput outputStructL = process(IN_L[i], &distChannelL);
    hw.WriteCvOut(1, 5.f * outputStructL.followerOutput);
    hw.WriteCvOut(2, 5.f * outputStructL.followerOutput);
    OUT_L[i] = outputStructL.audioOutput;
    OUT_R[i] = process(IN_R[i], &distChannelR).audioOutput;
  }
}

int main(void)
{
  pageFolder.pot2.setValue(0.5);
  pageFolder.pot3.setValue(0.5);

  pageClipper.pot1.setValue(0);
  pageClipper.pot2.setValue(0);

  hw.Init();
  hw.SetAudioBlockSize(8);
  hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_96KHZ);
  sampleRate = hw.AudioSampleRate();

  button.Init(hw.B7);

  distChannelL.init(sampleRate);
  distChannelR.init(sampleRate);

  //Set up oscillator
  osc.Init(sampleRate);
  osc.SetWaveform(osc.WAVE_SIN);
  osc.SetAmp(1.f);
  osc.SetFreq(1000);


  hw.StartAudio(AudioCallback);

  while (1) {
    readAllAdcInputs();
  }
}
