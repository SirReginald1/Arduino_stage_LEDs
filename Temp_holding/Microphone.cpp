/*
  ESP32 I2S Microphone Sample
  esp32-i2s-mic-sample.ino
  Sample sound from I2S microphone, display on Serial Plotter
  Requires INMP441 I2S microphone

  DroneBot Workshop 2022
  https://dronebotworkshop.com
*/
// Include I2S driver
#include <driver/i2s.h>

// Connections to INMP441 I2S microphone
#define I2S_WS 17
#define I2S_SD 19
#define I2S_SCK 18

// Use I2S Processor 0
#define I2S_PORT I2S_NUM_0

// Define input buffer length
#define bufferLen 64
int16_t sBuffer[bufferLen];

void i2s_install() {
  // Set up I2S Processor configuration
  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 192000,//44100,
    .bits_per_sample = i2s_bits_per_sample_t(16),
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT, //I2S_CHANNEL_FMT_ALL_RIGHT
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S), // If fails to compile try changing this line
    .intr_alloc_flags = 0, //ESP_INTR_FLAG_LEVEL1
    .dma_buf_count = 8, //2
    .dma_buf_len = bufferLen,
    .use_apll = false
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
}

void i2s_setpin() {
  // Set I2S pin configuration
  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };

  i2s_set_pin(I2S_PORT, &pin_config);
}

void setup() {

  // Set up Serial Monitor
  Serial.begin(115200);
  //Serial.println(" ");

  delay(1000);

  // Set up I2S
  i2s_install();
  i2s_setpin();
  i2s_start(I2S_PORT);


  delay(500);
}

void loop() {

  // Get I2S data and place in data buffer
  size_t bytesIn = 0;
  esp_err_t result = i2s_read(I2S_PORT, &sBuffer, bufferLen * sizeof(int16_t), &bytesIn, portMAX_DELAY);

  if (result == ESP_OK)
  {

    
    
    // Read I2S data buffer
    int16_t samples_read = bytesIn / 8;
    if (samples_read > 0) {
      //float mean = 0;
      for (int16_t i = 0; i < samples_read; ++i) {
        mean += (sBuffer[i]);
      }

      // Average the data reading
      mean /= samples_read;

      // Print to serial plotter
      Serial.println(mean);
    }
  }
}

/*
#include "driver/i2s.h"

#define I2S_WS 17
#define I2S_SD 19
#define I2S_SCK 18
#define SAMPLE_RATE 16000//44100
void setup() {
  // Initialize serial console
  Serial.begin(115200);

  // Configure I2S microphone
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
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
  i2s_read(I2S_NUM_0, samples, num_samples, &bytes_read, portMAX_DELAY);

  // Print audio data to serial console
  for (int i = 0; i < num_samples; i++) {
    //Serial.println(samples[i]);
    Serial.write(samples[i]);
  }
}
*/