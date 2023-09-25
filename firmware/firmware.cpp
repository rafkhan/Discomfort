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

void readAllAdcInputs()
{
  pot4 = hw.GetAdcValue(CV_4);
  cv1 = hw.GetAdcValue(CV_5);
  cv2 = hw.GetAdcValue(CV_6);
  cv3 = hw.GetAdcValue(CV_7);
  cv4 = hw.GetAdcValue(CV_8);
  
  pageEnvFollower.updatePage(
    hw.GetAdcValue(CV_1),
    hw.GetAdcValue(CV_2),
    hw.GetAdcValue(CV_3),
    pot4,
    hw.GetAdcValue(CV_5),
    hw.GetAdcValue(CV_6),
    hw.GetAdcValue(CV_7),
    hw.GetAdcValue(CV_8)
  );

  pageFolder.updatePage(
    hw.GetAdcValue(CV_1),
    hw.GetAdcValue(CV_2),
    hw.GetAdcValue(CV_3),
    pot4,
    hw.GetAdcValue(CV_5),
    hw.GetAdcValue(CV_6),
    hw.GetAdcValue(CV_7),
    hw.GetAdcValue(CV_8)
  );

  pageClipper.updatePage(
    hw.GetAdcValue(CV_1),
    hw.GetAdcValue(CV_2),
    hw.GetAdcValue(CV_3),
    pot4,
    hw.GetAdcValue(CV_5),
    hw.GetAdcValue(CV_6),
    hw.GetAdcValue(CV_7),
    hw.GetAdcValue(CV_8)
  );
}

DiscomfortOutput process(float audioIn, Discomfort *ch) {
  DiscomfortInput inputStruct = DiscomfortInput::create(audioIn);

  // inputStruct.setFolderValues(
  //   fclamp(pageFolder.pot1 + cv1, 0, 1),
  //   fclamp(pageFolder.pot2 + cv2, -1, 1),
  //   fclamp(pageFolder.pot3 + cv3, -1, 1)
  // );

  inputStruct.setClipperValues(
    fclamp(pageClipper.pot1 + cv1, 0, 1),
    fclamp(pageClipper.pot2 + cv2, 0, 1)
  );

  inputStruct.dryWet = pot4;

  // // // TODO multiplexers on these mfs
  // // inputStruct.filterBandA = pot1;
  // // inputStruct.filterBandB = pot2;
  // // inputStruct.filterBandC = pot3;
  // // inputStruct.filterBandD = pot4;
  // // inputStruct.filterBankType = FILTERBANK_ON;

  // // inputStruct.dryWet = cv1;
  // // inputStruct.foldGain = map(cv2, 0, 1, FOLDER_MIN_GAIN, FOLDER_MAX_GAIN);
  // // inputStruct.noiseVolume = cv3;
  // // inputStruct.clipperGain = cv4;

  return ch->process(inputStruct);
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
  hw.ProcessAllControls();
  for (size_t i = 0; i < size; i++)
  {
    DiscomfortOutput outputStructL = process(IN_L[i], &distChannelL);
    OUT_L[i] = outputStructL.audioOutput;
    OUT_R[i] = process(IN_R[i], &distChannelR).audioOutput;
  }
}

int main(void)
{
  pageFolder.pot2 = 0.5;
  pageFolder.pot3 = 0.5;
  pageFolder.cv1 = 0.5;

  hw.Init();
  hw.SetAudioBlockSize(8);
  hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_96KHZ);
  sampleRate = hw.AudioSampleRate();

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
