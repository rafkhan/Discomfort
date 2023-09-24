#ifndef DISCOMFORT_INPUT_H
#define DISCOMFORT_INPUT_H

#include "Follower.h"
#include "Folder.h"
#include "Clipper.h"
#include "FilterBank.h"

class DiscomfortInput {
  public:
    float input;
    float inputGain;
    float outputGain;

    // wavefolder
    float foldGain; // 0 ... N
    float foldOffset; // -1 ... +1
    float foldSymmetry; // -1 ... +1

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

    void setFolderValues(float gain, float offset, float symmetry) {
      this->foldGain = gain;
      this->foldOffset = offset;
      this->foldSymmetry = symmetry;
    }

    void setClipperValue(float gain) {
      this->clipperGain = gain;
    }


    static DiscomfortInput create(float audioInput) {
      DiscomfortInput dcInput;
      dcInput.input = audioInput;
      dcInput.inputGain = 1;
      dcInput.outputGain = 1;
      dcInput.foldGain = FOLDER_MIN_GAIN;
      dcInput.foldOffset = 0;
      dcInput.foldSymmetry = 0;
      dcInput.clipperGain = CLIPPER_MIN_GAIN;
      dcInput.clipperType = CLIPPER_SOFT;
      dcInput.crushValue = 0;
      dcInput.attack = 1;
      dcInput.decay = 50;
      dcInput.noiseVolume = 0;
      dcInput.noiseTone = 0;
      dcInput.filterBankType = FILTERBANK_OFF;
      dcInput.filterBandA = 0.75;
      dcInput.filterBandB = 0.75;
      dcInput.filterBandC = 0.75;
      dcInput.filterBandD = 0.75;
      dcInput.dryWet = 1;

      return dcInput;
    }
};

#endif