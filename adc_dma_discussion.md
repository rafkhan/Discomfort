# ADC and DMA Configuration Discussion

## Initial Problem
- Experiencing issues with ADC and DMA configuration
- Race condition when reading from buffer while it's being updated
- ADC was in continuous mode with DMA in circular mode

## Initial Solution Attempt
- Switched to one-shot mode for the multiplexer
- Modified ADC configuration to:
  - Stop ADC before reconfiguring
  - Initialize ADC channels using `InitSingle`
  - Start ADC after configuration

## Issues with Initial Solution
- Using `System::Delay` introduced audio artifacts
- Need to avoid sleeping/delays in audio processing
- Two potential approaches identified:
  1. Modify DMA configuration
  2. Disable DMA and read directly

## Proposed Solutions

### Direct ADC Reading Approach
- Disable DMA by using `OVS_NONE` (no oversampling)
- Remove `hw.adc.Start()` since not using DMA
- Remove all `System::Delay` calls
- Read ADC values directly when `GetFloat()` is called

### Audio Callback Considerations
- Audio callback runs at audio sample rate (e.g., 48kHz)
- Direct ADC reading in audio callback could be too slow
- Need to safely share ADC values between main loop and audio callback
- Potential solutions:
  - Use buffer or atomic variables for ADC values
  - Read ADC values at lower rate (e.g., every N samples)
  - Apply ADC-controlled processing without causing discontinuities

## Current Status
- Initial solution with delays was rejected due to audio artifacts
- Direct ADC reading approach proposed but not implemented
- Audio callback modifications discussed but not implemented
- Further discussion needed on optimal approach for audio processing 