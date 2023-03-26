#ifndef DISCOMFORT_H
#define DISCOMFORT_H

#include "daisysp.h"
#include "Follower.h"
#include "Clipper.h"
#include "FilterBank.h"

using namespace daisysp;

struct DiscomfortInput {
  float input;
  float inputGain;
  float outputGain;

  // wavefloder
  float foldGain;
  float foldOffset;
  float foldSymmetry;

  // Clipper
  float       clipperGain;
  ClipperType clipperType;

  // Crush
  float crushValue;

  // Follower
  float attack;
  float decay;
  float noiseVolume;
  float noiseTone;

  // Filter bank
  FilterBankType filterBankType;
  float          filterBandA;
  float          filterBandB;
  float          filterBandC;
  float          filterBandD;

  float dryWet;
};

class Discomfort {
  public:
    Discomfort();
    void init(float sampleRate);
    float process(DiscomfortInput input);
  private:
    Follower *follower;
    FilterBank *filterBank;
    WhiteNoise noise;
};

#endif