#include <cmath>

#include "Discomfort.h"
#include "Folder.h"
#include "Clipper.h"
#include "DryWet.h"
#include "DiscomfortInput.h"

Discomfort::Discomfort() {}

void Discomfort::init(float sampleRate)
{
  this->follower = new Follower(sampleRate, 1.0f, 1.0f);
  this->filterBank = new FilterBank(sampleRate);
  noise.Init();
}

DiscomfortOutput Discomfort::process(DiscomfortInput input)
{
  float gainStagedInput = input.input * input.inputGain;

  float foldOut = Folder::fold(gainStagedInput, input.foldGain, input.foldOffset, input.foldSymmetry) * 0.9;
  // output.audioOutput = foldOut;
  // output.followerOutput = 0;
  // float clippedOut = Clipper::clip(foldOut, input.clipperGain, input.clipperBend);

  // float followerAmplitude = this->follower->process(input.input, input.attack, input.decay);

  // float finalAudioOut = DryWet::blend(gainStagedInput, clippedOut, pow(input.dryWet, 3)) * 0.75;
  return this->createOutput(foldOut, 0);
}

DiscomfortOutput Discomfort::createOutput(float audio, float follower)
{
  DiscomfortOutput dcOutput;
  dcOutput.audioOutput = audio;
  dcOutput.followerOutput = follower;
  return dcOutput;
}
