/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "dht.h"
#include "i2c_lcd.h"
#include "ohms_law.h"

// ADC
// 12-bit conversion, assume max value == ADC_VREF == 3.3 V
const float ADC_CONVERSION_FACTOR = 3.3f / (1 << 12);
const int ADC_NUM_SAMPLES = 5;

// DHT
#ifdef PICO_DEFAULT_LED_PIN
#define LED_PIN PICO_DEFAULT_LED_PIN
#endif
const uint DHT_PIN = 15;

// LCD
#define MAX_LINES      2
#define MAX_CHARS      16
const unsigned char deg_sign[] = {0xDF, 0};

int main() {
  // ADC setup
  adc_init();
  adc_gpio_init(26);
  adc_select_input(0);


  // DHT setup
  stdio_init_all();
  gpio_init(DHT_PIN);
#ifdef LED_PIN
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
#endif

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
    #warning i2c/lcd_1602_i2c example requires a board with I2C pins
#else
  // Use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
  i2c_init(i2c_default, 100 * 1000);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
  // Make the I2C pins available to picotool
  bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

  lcd_init();

  char line1_text[MAX_CHARS];
  char line2_text[MAX_CHARS];
  for (;;) {
    // Read ADC
    // TODO: sample
    // uint16_t result = adc_read();
    float result = sampled_adc_read(ADC_NUM_SAMPLES);
    float result_v = result * ADC_CONVERSION_FACTOR;
    float voltage = voltage_divider(result_v, 10000.0, 2000.0);
    sprintf(line2_text, "V: %.2fv", voltage);

    // Read DHT
    dht_reading reading;
    read_from_dht(&reading, DHT_PIN);
    float fahrenheit = (reading.temp_celsius * 9 / 5) + 32;
    sprintf(line1_text, "T:%.0f%sF, RH:%.0f%%", fahrenheit, deg_sign, reading.humidity);

    // Draw LCD
    lcd_set_cursor(0, 0);
    lcd_string(line1_text);
    lcd_set_cursor(1, 0);
    lcd_string(line2_text);
    sleep_ms(2000);
    lcd_clear();
  }
  return 0;
#endif
}
