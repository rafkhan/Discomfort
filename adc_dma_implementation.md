# ADC and DMA Implementation

## Overview
This implementation uses DMA in normal mode with on-demand ADC triggering to avoid race conditions and audio artifacts while maintaining consistent timing with the audio sample rate.

## Implementation

### 1. ADC Initialization
```cpp
void initAdc()
{
  mux0.init(&muxSelect0, &muxSelect1, &muxSelect2, &hw, 0);
  mux1.init(&muxSelect0, &muxSelect1, &muxSelect2, &hw, 1);
  mux2.init(&muxSelect0, &muxSelect1, &muxSelect2, &hw, 2);

  // Stop ADC before reconfiguring
  hw.adc.Stop();

  // Configure ADC channels
  AdcChannelConfig adc[3];
  adc[0].InitSingle(hw.C7);
  adc[1].InitSingle(hw.C6);
  adc[2].InitSingle(hw.C5);

  // Initialize ADC with normal DMA mode
  hw.adc.Init(adc, 3);
  
  // Configure ADC for on-demand triggering
  ADC_HandleTypeDef* hadc = &hw.adc.hadc1;
  hadc->Init.ContinuousConvMode = DISABLE;  // Disable continuous mode
  hadc->Init.ExternalTrigConv = ADC_SOFTWARE_START;  // Trigger on software start
  
  // Reinitialize ADC with new settings
  HAL_ADC_Init(hadc);
}
```

### 2. ADC Trigger Function
```cpp
void triggerAdcRead()
{
  // Set multiplexer channels
  mux0.setBits(0);
  mux1.setBits(0);
  mux2.setBits(0);
  
  // Start ADC conversion
  HAL_ADC_Start_DMA(&hw.adc.hadc1, (uint32_t*)hw.adc.dma_buffer, 3);
}
```

### 3. Audio Callback Implementation
```cpp
void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
  static size_t sampleCounter = 0;
  const size_t ADC_READ_INTERVAL = 48; // Read every 1ms at 48kHz
  
  for (size_t i = 0; i < size; i++)
  {
    // Trigger ADC read at regular intervals
    if (sampleCounter == 0) {
      triggerAdcRead();
    }
    
    // Use the latest ADC values from DMA buffer
    float value0 = hw.adc.GetFloat(0);
    float value1 = hw.adc.GetFloat(1);
    float value2 = hw.adc.GetFloat(2);
    
    // Process audio using ADC values
    float processedL = IN_L[i] * value0;
    float processedR = IN_R[i] * value1;
    
    OUT_L[i] = processedL;
    OUT_R[i] = processedR;
    
    sampleCounter = (sampleCounter + 1) % ADC_READ_INTERVAL;
  }
}
```

## Key Features
1. Uses DMA in normal mode for reliable data transfer
2. Triggers ADC conversions on demand rather than continuously
3. Reads ADC values directly from the DMA buffer
4. Maintains consistent timing with the audio sample rate
5. Avoids any delays or sleeping

## Benefits
- No race conditions since DMA is in normal mode
- No audio artifacts from delays
- Efficient use of DMA for data transfer
- Consistent timing with audio processing 