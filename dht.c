#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

const uint MAX_TIMINGS = 85;

typedef struct {
  float humidity;
  float temp_celsius;
} dht_reading;

void read_from_dht(dht_reading *result, int dht_pin) {
  int data[5] = {0, 0, 0, 0, 0};
  uint last = 1;
  uint j = 0;

  gpio_set_dir(dht_pin, GPIO_OUT);
  gpio_put(dht_pin, 0);
  sleep_ms(18);
  gpio_put(dht_pin, 1);
  sleep_us(40);
  gpio_set_dir(dht_pin, GPIO_IN);

#ifdef LED_PIN
  gpio_put(LED_PIN, 1);
#endif
  for (uint i = 0; i < MAX_TIMINGS; i++) {
    uint count = 0;
    while (gpio_get(dht_pin) == last) {
      count++;
      sleep_us(1);
      if (count == 255) break;
    }
    last = gpio_get(dht_pin);
    if (count == 255) break;

    if ((i >= 4) && (i % 2 == 0)) {
      data[j / 8] <<= 1;
      if (count > 46) data[j / 8] |= 1;
      j++;
    }
  }
#ifdef LED_PIN
  gpio_put(LED_PIN, 0);
#endif
  if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
    // result->crc_match = true;
    result->humidity = (float) ((data[0] << 8) + data[1]) / 10;
    if (result->humidity > 100) {
      result->humidity = data[0];
    }
    result->temp_celsius = (float) (((data[2] & 0x7F) << 8) + data[3]) / 10;
    if (result->temp_celsius > 125) {
      result->temp_celsius = data[2];
    }
    if (data[2] & 0x80) {
      result->temp_celsius = -result->temp_celsius;
    }
  } else {
    // result->crc_match = false;
#ifdef DEBUG
    printf("{\"msg\": \"CRC mismatch - crc:%i actual:%i\"}\n", data[4], ((data[0] + data[1] + data[2] + data[3]) & 0xFF));
#endif
  }
}

void read_from_dht_original(dht_reading *result, int dht_pin) {
    int data[5] = {0, 0, 0, 0, 0};
    uint last = 1;
    uint j = 0;


    gpio_set_dir(dht_pin, GPIO_OUT);
    gpio_put(dht_pin, 0);
    sleep_ms(18);
    gpio_put(dht_pin, 1);
    sleep_us(40);
    gpio_set_dir(dht_pin, GPIO_IN);

#ifdef LED_PIN
    gpio_put(LED_PIN, 1);
#endif
    for (uint i = 0; i < MAX_TIMINGS; i++) {
        uint count = 0;
        while (gpio_get(dht_pin) == last) {
            count++;
            sleep_us(1);
            if (count == 255) break;
        }
        last = gpio_get(dht_pin);
        if (count == 255) break;

        if ((i >= 4) && (i % 2 == 0)) {
            data[j / 8] <<= 1;
            if (count > 16) data[j / 8] |= 1;
            j++;
        }
    }
#ifdef LED_PIN
    gpio_put(LED_PIN, 0);
#endif

    if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
        result->humidity = (float) ((data[0] << 8) + data[1]) / 10;
        if (result->humidity > 100) {
            result->humidity = data[0];
        }
        result->temp_celsius = (float) (((data[2] & 0x7F) << 8) + data[3]) / 10;
        if (result->temp_celsius > 125) {
            result->temp_celsius = data[2];
        }
        if (data[2] & 0x80) {
            result->temp_celsius = -result->temp_celsius;
        }
    } else {
        printf("Bad data\n");
    }
}


