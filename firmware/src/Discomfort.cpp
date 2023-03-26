#include "Discomfort.h"
#include "Folder.h"
#include "Clipper.h"
#include "DryWet.h"

Discomfort::Discomfort() {}

void Discomfort::init(float sampleRate) {
  this->follower = new Follower(sampleRate, 1.0f, 30.0f);
  this->filterBank = new FilterBank(sampleRate);
  noise.Init();
}

float Discomfort::process(DiscomfortInput input) {
  float gainStagedInput = input.input * input.inputGain;

  float foldOut = Folder::fold(gainStagedInput, input.foldGain, input.foldOffset);
  float clippedOut = Clipper::clip(foldOut, input.clipperGain, input.clipperType);

  float followerAmplitude = this->follower->process(input.input, input.attack, input.decay);
  float noiseValue = noise.Process() * followerAmplitude * input.noiseVolume;

  float mix = (clippedOut + noiseValue) * input.outputGain;
  float eqMix = filterBank->process(mix, input.filterBankType, input.filterBandA, input.filterBandB, input.filterBandC, input.filterBandD);

  return DryWet::blend(gainStagedInput, eqMix, input.dryWet);
}
