typedef struct {
  float humidity;
  float temp_celsius;
} dht_reading;

void read_from_dht(dht_reading *result, int dht_pin);
