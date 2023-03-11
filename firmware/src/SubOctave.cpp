#include "SubOctave.h"

SubOctave::SubOctave(float sampleRate) {
  osc.Init(sampleRate);
  osc.SetAmp(1.0f);
  osc.SetWaveform(osc.WAVE_POLYBLEP_SQUARE);
}

float SubOctave::play(float input, float tone) {
  return osc.Process();
}
