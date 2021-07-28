#include "hardware/adc.h"

float voltage_divider(float vin, float r1, float r2) {
  return (vin * (r1 + r2) / r2);
}

float sampled_adc_read(int sample_cnt) {
  float sample_sum;
  for(int i = 0; i < sample_cnt; i++) {
    uint16_t result = adc_read();
    sample_sum += result;
  }
  return sample_sum / sample_cnt;
}
