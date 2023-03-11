#ifndef DISCOMFORT_H
#define DISCOMFORT_H

#include "daisysp.h"
#include "Follower.h"
#include "Clipper.h"
#include "FilterBank.h"

using namespace daisysp;

class Discomfort {
  public:
    Discomfort();
    void init(float sampleRate);
    float process(
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
    );
  private:
    Follower *follower;
    FilterBank *filterBank;
    WhiteNoise noise;
};

#endif