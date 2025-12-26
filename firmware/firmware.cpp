#include "daisy_patch_sm.h"
#include "daisysp.h"

#include "src/DiscomfortInternal/Discomfort.h"
#include "src/DiscomfortInternal/util.h"
#include "src/DiscomfortInternal/Clipper.h"
#include "src/DiscomfortInternal/Folder.h"
#include "src/DiscomfortInternal/FilterBank.h"
#include "src/DiscomfortInternal/DryWet.h"

#include "src/hardware/mux.h"
#include "src/hardware/discomfortHwInputs.h"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

float sampleRate;
DaisyPatchSM hw;

Discomfort distChannelL;
Discomfort distChannelR;

float a = 0;
float b = 0;
float c = 0;
float d = 0;
float e = 0;
float f = 0;

// Can move into mux class probably
dsy_gpio muxSelect0;
dsy_gpio muxSelect1;
dsy_gpio muxSelect2;

Mux mux0;
Mux mux1;
Mux mux2;
Mux *muxes[3] = {&mux0, &mux1, &mux2};
DiscomfortHwInputs *hardwareInputs;
int muxPinIdx = 0; // for iterating over the all mux select pins
Oscillator osc;

float asd[4] = { 0, 0, 0, 0 };

DiscomfortHwAnalogInput* testInput = nullptr;

void initAdc() {}

DiscomfortOutput process(float audioIn, DiscomfortHwInputs *hwInputs, Discomfort *ch)
{
  DiscomfortInput inputStruct = hardwareInputs->createDiscomfortInput(audioIn);
  return ch->process(inputStruct);
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
  for (size_t i = 0; i < size; i++)
  {
    OUT_L[i] = osc.Process();
    OUT_R[i] = IN_R[i];
  }
}

void printThing(DiscomfortHwAnalogInput *x) {
    hw.PrintLine(
      "(%d, %d): %f\t",
      x->muxIdx,
      x->muxPin,
      x->getValue()
    );
}

void InitADCPolling() {
  dsy_adc& adc = *okay_getDsyAdc();
  HAL_ADC_Stop_DMA(&adc.hadc1);
  adc.hadc1.Init.ContinuousConvMode = DISABLE;
  adc.hadc1.Init.DiscontinuousConvMode = ENABLE;
  adc.hadc1.Init.NbrOfDiscConversion = 1;
  HAL_ADC_Init(&adc.hadc1);
}

void setMuxPins(int muxPinIdx) {
    dsy_gpio_write(&muxSelect0, (bool) ((muxPinIdx >> 0) & 1));
    dsy_gpio_write(&muxSelect1, (bool) ((muxPinIdx >> 1) & 1));
    dsy_gpio_write(&muxSelect2, (bool) ((muxPinIdx >> 2) & 1));
}

// void ConfigureADCChannel(ADC_HandleTypeDef* hadc, uint8_t channel) {
//     ADC_ChannelConfTypeDef sConfig = {0};
//     sConfig.Channel = channel;
//     sConfig.Rank = ADC_REGULAR_RANK_1;
//     sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
//     HAL_ADC_ConfigChannel(hadc, &sConfig);
// }

// void StartADCConversion(ADC_HandleTypeDef* hadc) {
//     HAL_ADC_Start(hadc);
// }

// uint16_t PollADCConversion(ADC_HandleTypeDef* hadc) {
//     HAL_ADC_PollForConversion(hadc, 2);
//     return HAL_ADC_GetValue(hadc);
// }

// uint16_t ReadADC(uint8_t channel) {
//     ADC_HandleTypeDef* hadc = &okay_getDsyAdc()->hadc1;
    
//     ConfigureADCChannel(hadc, channel);
//     StartADCConversion(hadc);
//     return PollADCConversion(hadc);
// }

static const uint32_t dsy_adc_channel_map[DSY_ADC_MAX_CHANNELS] = {
    ADC_CHANNEL_3,
    ADC_CHANNEL_4,
    ADC_CHANNEL_5,
    ADC_CHANNEL_7,
    ADC_CHANNEL_8,
    ADC_CHANNEL_9,
    ADC_CHANNEL_10,
    ADC_CHANNEL_11,
    ADC_CHANNEL_12,
    ADC_CHANNEL_13,
    ADC_CHANNEL_14,
    ADC_CHANNEL_15,
    ADC_CHANNEL_16,
    ADC_CHANNEL_17,
    ADC_CHANNEL_18,
    ADC_CHANNEL_19,
};

uint8_t GetADCChannelIndex(const Pin& pin) {
    // Map pin to ADC channel index based on the dsy_adc_channel_map array ordering
    // These mappings are based on the PIN_CHN_x definitions in libdaisy
    
    if (pin.port == PORTA && pin.pin == 6) return 0;   // PA6 -> Channel 3 -> Index 0
    if (pin.port == PORTC && pin.pin == 4) return 1;   // PC4 -> Channel 4 -> Index 1
    if (pin.port == PORTB && pin.pin == 1) return 2;   // PB1 -> Channel 5 -> Index 2
    if (pin.port == PORTA && pin.pin == 7) return 3;   // PA7 -> Channel 7 -> Index 3
    if (pin.port == PORTC && pin.pin == 5) return 4;   // PC5 -> Channel 8 -> Index 4
    if (pin.port == PORTB && pin.pin == 0) return 5;   // PB0 -> Channel 9 -> Index 5
    if (pin.port == PORTC && pin.pin == 0) return 6;   // PC0 -> Channel 10 -> Index 6
    if (pin.port == PORTC && pin.pin == 1) return 7;   // PC1 -> Channel 11 -> Index 7
    if (pin.port == PORTC && pin.pin == 2) return 8;   // PC2 -> Channel 12 -> Index 8
    if (pin.port == PORTC && pin.pin == 3) return 9;   // PC3 -> Channel 13 -> Index 9
    if (pin.port == PORTA && pin.pin == 2) return 10;  // PA2 -> Channel 14 -> Index 10
    if (pin.port == PORTA && pin.pin == 3) return 11;  // PA3 -> Channel 15 -> Index 11
    if (pin.port == PORTA && pin.pin == 0) return 12;  // PA0 -> Channel 16 -> Index 12
    if (pin.port == PORTA && pin.pin == 1) return 13;  // PA1 -> Channel 17 -> Index 13
    if (pin.port == PORTA && pin.pin == 4) return 14;  // PA4 -> Channel 18 -> Index 14
    if (pin.port == PORTA && pin.pin == 5) return 15;  // PA5 -> Channel 19 -> Index 15
    
    return 0xFF; // Invalid pin - not an ADC pin
}

uint8_t GetPatchSMAnalogChannelIndex(int cv_input) {
    // Map DaisyPatchSM CV inputs to ADC channel indices
    // You'll need to check the DaisyPatchSM schematic for exact pin mappings
    
    switch(cv_input) {
        case 1:  // C1 - check actual pin mapping
            return GetADCChannelIndex(DaisyPatchSM::C1);
        case 2:  // C2
            return GetADCChannelIndex(DaisyPatchSM::C2);
        case 3:  // C3
            return GetADCChannelIndex(DaisyPatchSM::C3);
        case 4:  // C4
            return GetADCChannelIndex(DaisyPatchSM::C4);
        case 5:  // C5
            return GetADCChannelIndex(DaisyPatchSM::C5);
        case 6:  // C6
            return GetADCChannelIndex(DaisyPatchSM::C6);
        case 7:  // C7
            return GetADCChannelIndex(DaisyPatchSM::C7);
        case 8:  // C8
            return GetADCChannelIndex(DaisyPatchSM::C8);
        // Add more as needed
        default:
            return 0xFF; // Invalid
    }
}

// 2. Polling read function
uint16_t ReadADCPolling(uint8_t channel) {
    dsy_adc& adc = *okay_getDsyAdc();
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = dsy_adc_channel_map[channel];
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
    
    HAL_ADC_ConfigChannel(&adc.hadc1, &sConfig);
    
    // Start conversion
    HAL_ADC_Start(&adc.hadc1);
    
    // Poll for conversion complete
    HAL_ADC_PollForConversion(&adc.hadc1, 10);
    
    // Get the value
    return HAL_ADC_GetValue(&adc.hadc1);
}

// Usage example:
uint16_t ReadPatchSMAnalogPin(const Pin& pin) {
    uint8_t channel_index = GetADCChannelIndex(pin);
    if (channel_index == 0xFF) {
        return 69; // Invalid pin
    }
    return ReadADCPolling(channel_index);
}

int main(void)
{
  hw.Init();
  hw.adc.Stop();

  dsy_gpio_pin pin = hw.C7;
  dsy_gpio gpio;
  gpio.pin = pin;
	gpio.mode = DSY_GPIO_MODE_ANALOG;
	gpio.pull = DSY_GPIO_NOPULL;
	dsy_gpio_init(&gpio);

  InitADCPolling();

  // dsy_adc& adc = *okay_getDsyAdc();
  uint32_t channel = adc_channel_from_pin(&pin);

  AdcChannelConfig adc[1];
  adc[0].InitSingle(hw.C7);
  // adc[1].InitSingle(hw.C6);
  // adc[2].InitSingle(hw.C5);
  hw.adc.Init(adc, 1);
  hw.adc.Start();

  mux0.init(&muxSelect0, &muxSelect1, &muxSelect2, &hw, 0);
  mux1.init(&muxSelect0, &muxSelect1, &muxSelect2, &hw, 1);
  mux2.init(&muxSelect0, &muxSelect1, &muxSelect2, &hw, 2);

  hw.StartLog();
  System::Delay(100);

  hw.SetAudioBlockSize(8);
  hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
  sampleRate = hw.AudioSampleRate();

  distChannelL.init(sampleRate);
  distChannelR.init(sampleRate);

  hw.StartAudio(AudioCallback);
  Mux *muxes[3] = {&mux0, &mux1, &mux2};
  hardwareInputs = new DiscomfortHwInputs(&hw, muxes);

  osc.Init(sampleRate);
  osc.SetWaveform(Oscillator::WAVE_SIN);
  osc.SetFreq(220);
  osc.SetAmp(0.5);

  int init = 0;
  setMuxPins(init);

  while (1)
  {
    hw.PrintLine("%d | %.3f %.3f %.3f | %.3f %.3f %.3f", channel, a, b, c, d, e, f);

    setMuxPins(muxPinIdx);
    System::Delay(1);
    switch(muxPinIdx) {
      case 0:
        a = ReadPatchSMAnalogPin(hw.C7);
        break;
      case 1:
        b = ReadPatchSMAnalogPin(hw.C7);
        break;
      case 2:
        c = ReadPatchSMAnalogPin(hw.C7);
        break;
      case 3:
        d = ReadPatchSMAnalogPin(hw.C7);
        break;
      case 4:
        e = ReadPatchSMAnalogPin(hw.C7);
        break;
      case 5:
        f = ReadPatchSMAnalogPin(hw.C7);
        break;
    }
    muxPinIdx++;
    if(muxPinIdx == 8) {
      muxPinIdx = 0;
    }

    // a = hw.adc.GetFloat(0);
    // b = hw.adc.GetFloat(1); // envelope knob
    // c = hw.adc.GetFloat(2); // channel 2 not connected to anything?

    //System::Delay(1);

    // muxPinIdx = 1;
    // setMuxPins(muxPinIdx);
    
    // // d = hw.adc.GetFloat(0); // fold symmetry knob
    // // e = hw.adc.GetFloat(1); // dist amount knob
    // // f = hw.adc.GetFloat(2); // channel 2 not connected to anything?

    // System::Delay(1);
  }
}
