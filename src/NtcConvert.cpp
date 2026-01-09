#include "NtcConvert.h"
#include <math.h>

static inline int clampInt(int v, int lo, int hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

float NtcConvert(int adc)
{
    // --- Costanti impianto 
    constexpr float ADC_MAX  = 4095.0f;     // INGRESSO ADC DA MCP3202
    constexpr float VREF     = 3.3f;        // riferimento ADC 3V3
    constexpr float RTOP_OHM = 33000.0f;    // RESISTENZA DA 3V3 -> nodo ADC
    constexpr float BETA     = 3435.0f;     // NTC Beta
    constexpr float R0_OHM   = 10000.0f;    // 10k @ 25°C
    constexpr float T0_K     = 298.15f;     // 25°C in Kelvin

   
    adc = clampInt(adc, 0, 4095);

   
    if (adc <= 0 || adc >= 4095) {
        return NAN;
    }

    // ADC -> tensione
    const float v = (static_cast<float>(adc) / ADC_MAX) * VREF;

    // Ricavo Rntc dal partitore: V = Vref * Rntc/(Rtop + Rntc)
    // => Rntc = Rtop * V / (Vref - V)
    const float denom = (VREF - v);
    if (denom <= 0.0f) {
        return NAN;
    }
    const float rntc = RTOP_OHM * (v / denom);
    if (!(rntc > 0.0f)) {
        return NAN;
    }

    // Modello Beta:
    // 1/T = 1/T0 + (1/B) * ln(R/R0)
    const float invT = (1.0f / T0_K) + (1.0f / BETA) * logf(rntc / R0_OHM);
    if (!(invT > 0.0f)) {
        return NAN;
    }

    const float tempK = 1.0f / invT;
    return tempK - 273.15f;
}
