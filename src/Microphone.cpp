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
#define I2S_WS 27
#define I2S_SD 33
#define I2S_SCK 32

// Define input buffer length
#define bufferLen 1024

/** The size of the sample used to performe transformations */
#define hopLength 512

// Use I2S Processor 0
#define I2S_PORT I2S_NUM_0

class Microphone{
    public:

        static void i2s_install(int sampleRate = 44100);

        static void i2s_setpin();

        static void setup(int sampleRate = 44100);

        template<typename T> static size_t readMic(void* buffer);
            
};

void Microphone::i2s_install(int sampleRate) {
    // Set up I2S Processor configuration
    const i2s_config_t i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = sampleRate,
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

void Microphone::i2s_setpin() {
    // Set I2S pin configuration
    const i2s_pin_config_t pin_config = {
      .bck_io_num = I2S_SCK,
      .ws_io_num = I2S_WS,
      .data_out_num = -1,
      .data_in_num = I2S_SD
    };

    i2s_set_pin(I2S_PORT, &pin_config);
}

void Microphone::setup(int sampleRate) {
    i2s_install(sampleRate);
    i2s_setpin();
    i2s_start(I2S_PORT);
}
/*
size_t Microphone::readMic(void* buffer){
    // Get I2S data and place in data buffer
    size_t bytesIn = 0;
    i2s_read(I2S_PORT, buffer, bufferLen * sizeof(int16_t), &bytesIn, portMAX_DELAY);
    return bytesIn;
}*/

/*
void testSampleRate(){
  newTime = micros();
  int16_t buffer[1000000];
  
  // Do 1 million reads and record time taken
  //for (int i = 0; i < 1000000; i++) {
  i2s_read(I2S_PORT, &buffer, 1000000 * sizeof(int16_t), &bytesIn, portMAX_DELAY);
  //}

  float conversionTime = (micros() - newTime) / 1000000.0;

  Serial.print("Conversion time: ");
  Serial.print(conversionTime);
  Serial.println(" uS");

  Serial.print("Max sampling frequency: ");
  Serial.print((1.0 / conversionTime) * 1000000);
  Serial.println(" Hz");
}
*/