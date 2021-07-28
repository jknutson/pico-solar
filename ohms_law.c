#include "hardware/adc.h"

float voltage_divider(float vin, float r1, float r2) {
  return (vin * (r1 + r2) / r2);
}

float sampled_adc_read(int sample_cnt) {
  float sample_sum = 0.0f;
  for(int i = 0; i < sample_cnt; i++) {
    sample_sum += adc_read();
  }
  return sample_sum / sample_cnt;
}
