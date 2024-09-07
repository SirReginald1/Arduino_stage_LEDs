#include "driver/i2s.h"

#define I2S_WS 17
#define I2S_SD 19
#define I2S_SCK 18
#define SAMPLE_RATE 44100
void setup() {
  // Initialize serial console
  Serial.begin(115200);

  // Configure I2S microphone
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 2,
    .dma_buf_len = 64
  };
  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_set_clk(I2S_NUM_0, SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}

void loop() {
  // Allocate buffer to store audio data
  const int num_samples = 512;
  int16_t samples[num_samples];

  // Read audio data from I2S microphone
  size_t bytes_read = 0;
  i2s_read(I2S_NUM_0, samples, num_samples * sizeof(int16_t), &bytes_read, portMAX_DELAY);

  // Print audio data to serial console
  for (int i = 0; i < num_samples; i++) {
    //Serial.println(samples[i]);
    Serial.write(samples,num_samples);
  }
}