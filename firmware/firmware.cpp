#include "daisy_patch_sm.h"
#include "daisysp.h"

#include "src/Discomfort.h"
#include "src/util.h"
#include "src/Clipper.h"
#include "src/Folder.h"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

DaisyPatchSM hw;
float sampleRate;

Discomfort distChannelL;
Discomfort distChannelR;

float pot1 = 0;
float pot2 = 0;
float pot3 = 0;
float pot4 = 0;

float cv1 = 0;
float cv2 = 0;
float cv3 = 0;
float cv4 = 0;

void readAllAdcInputs() {
	pot1 = hw.GetAdcValue(CV_1);
	pot2 = hw.GetAdcValue(CV_2);
	pot3 = hw.GetAdcValue(CV_3);
	pot4 = hw.GetAdcValue(CV_4);

	cv1 = hw.GetAdcValue(CV_5);
	cv2 = hw.GetAdcValue(CV_6);
	cv3 = hw.GetAdcValue(CV_7);
	cv4 = hw.GetAdcValue(CV_8);
}

float process(float input, Discomfort *ch) {
	return ch->process(
		input,
		1,
		1,

		1, // map(foldGainPotInput, 0, 1, 1, FOLDER_MAX_GAIN),
		0,

		1, // map(shaperGainPotInput, 0, 1, 1, CLIPPER_MAX_GAIN),
		CLIPPER_SOFT,

		0,

		0,
		0,
		0, // map(tiltPotInput, 0, 1, 0, 1),

		FILTERBANK_ON,
		0,
		0,
		0,
		0,

		1
	);
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	hw.ProcessAllControls();
	for (size_t i = 0; i < size; i++) {
		OUT_L[i] = process(IN_L[i], &distChannelL);
		OUT_R[i] = process(IN_R[i], &distChannelR);
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(2); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_96KHZ);
	sampleRate = hw.AudioSampleRate();

	distChannelL.init(sampleRate);
	distChannelR.init(sampleRate);

	hw.StartAudio(AudioCallback);
	while(1) {
		readAllAdcInputs();
	}
}
