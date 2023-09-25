#include "Discomfort.h"
#include "Folder.h"
#include "Clipper.h"
#include "DryWet.h"
#include "DiscomfortInput.h"

Discomfort::Discomfort() {}

void Discomfort::init(float sampleRate) {
  this->follower = new Follower(sampleRate, 1.0f, 1.0f);
  this->filterBank = new FilterBank(sampleRate);
  noise.Init();
}

DiscomfortOutput Discomfort::process(DiscomfortInput input) {
  float gainStagedInput = input.input * input.inputGain;

  float foldOut = Folder::fold(gainStagedInput, input.foldGain, input.foldOffset, input.foldSymmetry);
  float clippedOut = Clipper::clip(foldOut, input.clipperGain, input.clipperType);

  float followerAmplitude = this->follower->process(input.input, input.attack, input.decay);
  float noiseValue = noise.Process() * followerAmplitude * input.noiseVolume;

  float mix = (clippedOut + noiseValue) * input.outputGain;

  // float eqMix = filterBank->process(
  //   mix,
  //   input.filterBankType,
  //   input.filterBandA,
  //   input.filterBandB,
  //   input.filterBandC,  
  //   input.filterBandD
  // );

  float finalAudioOut = DryWet::blend(gainStagedInput, mix, input.dryWet);

  return this->createOutput(finalAudioOut, followerAmplitude);
}

DiscomfortOutput Discomfort::createOutput(float audio, float follower) {
  DiscomfortOutput dcOutput;
  dcOutput.audioOutput = audio;
  dcOutput.followerOutput = follower;
  return dcOutput;
}
