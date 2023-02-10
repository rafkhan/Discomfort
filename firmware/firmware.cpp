#include "daisy_patch_sm.h"
#include "daisysp.h"

#include "Discomfort.h"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

DaisyPatchSM hw;

Discomfort distChannelL;
Discomfort distChannelR;

float foldGainPotInput = 0;
float shaperGainPotInput = 0;
float tiltPotInput = 0;
float dryWetPotInput = 0;

#define FOLDER_MAX_GAIN 50
#define SHAPER_MAX_GAIN 20

float mapFFFF(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void readAllAdcInputs() {
	foldGainPotInput = hw.GetAdcValue(CV_1);
	shaperGainPotInput = hw.GetAdcValue(CV_2);
	tiltPotInput = hw.GetAdcValue(CV_3);
	dryWetPotInput = hw.GetAdcValue(CV_4);
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	hw.ProcessAllControls();
	for (size_t i = 0; i < size; i++)
	{
		OUT_L[i] = distChannelL.process(
			IN_L[i],
			mapFFFF(foldGainPotInput, 0, 1, 1, FOLDER_MAX_GAIN),
			mapFFFF(shaperGainPotInput, 0, 1, 1, SHAPER_MAX_GAIN),
			tiltPotInput,
			mapFFFF(dryWetPotInput, 0, 1, 0, 1)
		);

		OUT_R[i] = distChannelR.process(
			IN_R[i],
			mapFFFF(foldGainPotInput, 0, 1, 1, FOLDER_MAX_GAIN),
			mapFFFF(shaperGainPotInput, 0, 1, 1, SHAPER_MAX_GAIN),
			tiltPotInput,
			dryWetPotInput
		);
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(2); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);
	while(1) {
		readAllAdcInputs();
	}
}
