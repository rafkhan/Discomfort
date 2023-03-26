#include "daisy_patch_sm.h"
#include "daisysp.h"

#include "src/Discomfort.h"
#include "src/util.h"
#include "src/Clipper.h"
#include "src/Folder.h"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

DaisyPatchSM hw;
float sampleRate;

Discomfort distChannelL;
Discomfort distChannelR;

float pot1 = 0;
float pot2 = 0;
float pot3 = 0;
float pot4 = 0;

float cv1 = 0;
float cv2 = 0;
float cv3 = 0;
float cv4 = 0;

void readAllAdcInputs()
{
  pot1 = hw.GetAdcValue(CV_1);
  pot2 = hw.GetAdcValue(CV_2);
  pot3 = hw.GetAdcValue(CV_3);
  pot4 = hw.GetAdcValue(CV_4);

  cv1 = hw.GetAdcValue(CV_5);
  cv2 = hw.GetAdcValue(CV_6);
  cv3 = hw.GetAdcValue(CV_7);
  cv4 = hw.GetAdcValue(CV_8);
}

DiscomfortInput GET_DEFAULT_INPUT(float input)
{
  DiscomfortInput dcInput;
  dcInput.input = input;
  dcInput.inputGain = 1;
  dcInput.outputGain = 1;
  dcInput.foldGain = FOLDER_MIN_GAIN;
  dcInput.foldOffset = 0;
  dcInput.foldSymmetry = 0;
  dcInput.clipperGain = 0;
  dcInput.clipperType = CLIPPER_SOFT;
  dcInput.crushValue = 0;
  dcInput.attack = 1; // ms
  dcInput.decay = 50; // ms
  dcInput.noiseVolume = 0.5;
  dcInput.noiseTone = 0;
  dcInput.filterBankType = FILTERBANK_ON;
  dcInput.filterBandA = 0.75;
  dcInput.filterBandB = 0.75;
  dcInput.filterBandC = 0.75;
  dcInput.filterBandD = 0.75;
  dcInput.dryWet = 1;

  return dcInput;
}

float process(float input, Discomfort *ch)
{
  cv1 = std::clamp(cv1, 0.0f, 1.0f);
  cv2 = std::clamp(cv2, 0.0f, 1.0f);
  cv3 = std::clamp(cv3, 0.0f, 1.0f);
  cv4 = std::clamp(cv4, 0.0f, 1.0f);

  DiscomfortInput inputStruct = GET_DEFAULT_INPUT(input);

  // TODO multiplexers on these mfs
  inputStruct.filterBandA = pot1;
  inputStruct.filterBandB = pot2;
  inputStruct.filterBandC = pot3;
  inputStruct.filterBandD = pot4;

  inputStruct.dryWet = cv1;
  inputStruct.foldGain = cv2;
  inputStruct.noiseVolume = cv3;
  inputStruct.clipperGain = cv4;

  return ch->process(inputStruct);
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
  hw.ProcessAllControls();
  for (size_t i = 0; i < size; i++)
  {
    OUT_L[i] = process(IN_L[i], &distChannelL);
    OUT_R[i] = process(IN_R[i], &distChannelR);
  }
}

int main(void)
{
  hw.Init();
  hw.SetAudioBlockSize(32);
  hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_96KHZ);
  sampleRate = hw.AudioSampleRate();

  distChannelL.init(sampleRate);
  distChannelR.init(sampleRate);

  hw.StartAudio(AudioCallback);
  while (1)
  {
    readAllAdcInputs();
  }
}
