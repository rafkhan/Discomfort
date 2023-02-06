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
			foldGainPotInput,
			shaperGainPotInput,
			tiltPotInput,
			dryWetPotInput
		);

		OUT_R[i] = distChannelL.process(
			IN_R[i],
			foldGainPotInput,
			shaperGainPotInput,
			tiltPotInput,
			dryWetPotInput
		);
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);
	while(1) {
		readAllAdcInputs();
	}
}
