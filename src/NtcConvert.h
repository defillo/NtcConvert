#pragma once
#include <Arduino.h>

// Unica API pubblica: ADC 0..4095 -> Temperatura (°C)
float NtcConvert(int adc);
