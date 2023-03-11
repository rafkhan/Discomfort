#include "Discomfort.h"
#include "Folder.h"
#include "Clipper.h"
#include "DryWet.h"

Discomfort::Discomfort() {}

void Discomfort::init(float sampleRate) {
  this->follower = new Follower(sampleRate, 1.0f, 30.0f);
  noise.Init();
}

float Discomfort::process(
      float input,
      float inputGain,
      float outputGain,

      // wavefloder
      float foldGain,
      float foldOffset,

      // Clipper
      float       shaperGain,
      ClipperType clipperType,

      // Crush
      float crushValue,
  
      // Follower
      float attack,
      float decay,
      float noiseVolume,

      // Filter bank
      FilterBankType filterBankType,
      float          filterBandA,
      float          filterBandB,
      float          filterBandC,
      float          filterBandD,

      float dryWet
) {
  input = input * inputGain;

  float foldOut = Folder::fold(input, foldGain, foldOffset);
  float clippedOut = Clipper::clip(foldOut, shaperGain, clipperType);

  float followerAmplitude = this->follower->process(input, attack, decay);
  float noiseValue = noise.Process() * followerAmplitude * noiseVolume;

  float mix = (clippedOut + noiseValue) * outputGain;

  return DryWet::blend(input, mix, dryWet);
}
