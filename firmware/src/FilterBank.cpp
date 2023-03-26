#include "FilterBank.h"
#include "util.h"

#include "daisysp.h"
using namespace daisysp;

FilterBankBand::FilterBankBand(float sampleRate, float lowFreq, float highFreq) {
  lowPass.Init(sampleRate);
  lowPass.SetFreq(lowFreq);
  lowPass.SetRes(0.5);

  highPass.Init(sampleRate);
  highPass.SetFreq(highFreq);
  highPass.SetRes(0.5);
}

float FilterBankBand::process(float input, float level) {
  float gain = 1;

  if (level < 0.75) {
    gain = map(level, 0, 0.75, 0, 1);
  } else {
    float drive = map(1.0f - level, 0, 0.25, 1, 5);
    highPass.SetDrive(drive);
  }

  lowPass.Process(input);
  highPass.Process(lowPass.Low());
  float output = highPass.High();

  return output * gain;
}

FilterBank::FilterBank(float sampleRate) {
  bandA = new FilterBankBand(sampleRate, 20, 250);
  bandB = new FilterBankBand(sampleRate, 250, 1000);
  bandC = new FilterBankBand(sampleRate, 1000, 10000);
  bandD = new FilterBankBand(sampleRate, 10000, 25000);
}

float FilterBank::process(
  float input,
  FilterBankType type,
  float levelA,
  float levelB,
  float levelC,
  float levelD
) {
  float outA = bandA->process(input, levelA);
  float outB = bandB->process(input, levelB);
  float outC = bandC->process(input, levelC);
  float outD = bandD->process(input, levelD);

  return outA + outB + outC + outD;
}
