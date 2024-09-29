#ifndef Audio_h
#define Audio_h
#include <driver/i2s.h>
#include "Globals.h"
#include <Arduino.h>

// Define input buffer length
#define bufferLen 1024

// Connections to INMP441 I2S microphone
#define I2S_WS 27 // (LRCK)
#define I2S_SCK 32 // (BCK)
#define I2S_SD_MIC 33 // (Data for Microphone)
#define I2S_SD_ADC 26    // (Data for ADC)

// Define input buffer length
#define BUFFER_LEN_MIC 1024
#define BUFFER_LEN_ADC 1024

#define BITS_PER_SAMPLE_MIC 16
#define BITS_PER_SAMPLE_ADC 16

#define SAMPLE_RATE_ADC 48000//44100
#define SAMPLE_RATE_MIC 44100

/** The size of the sample used to performe transformations */
//#define hopLength 512

// Use I2S Processor 0
#define I2S_PORT_MIC I2S_NUM_0
#define I2S_PORT_ADC I2S_NUM_1


class Audio{
    public:

        static i2s_port_t currentPort;

        static size_t currentBufferLength;

        static void i2s_mic_install() {
            // Set up I2S Processor configuration
            const i2s_config_t i2s_config = {
                .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
                .sample_rate = SAMPLE_RATE_MIC,
                .bits_per_sample = i2s_bits_per_sample_t(BITS_PER_SAMPLE_MIC),
                .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT, //I2S_CHANNEL_FMT_ALL_RIGHT
                .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S), // If fails to compile try changing this line
                .intr_alloc_flags = ESP_INTR_FLAG_LEVEL2, //ESP_INTR_FLAG_LEVEL1
                .dma_buf_count = 8, //2
                .dma_buf_len = BUFFER_LEN_MIC,
                .use_apll = false
            };
            i2s_driver_install(I2S_PORT_MIC, &i2s_config, 0, NULL);
        }

        static void i2s_adc_install() {
            // Set up I2S Processor configuration
            const i2s_config_t i2s_config = {
                .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
                .sample_rate = SAMPLE_RATE_ADC,
                .bits_per_sample = i2s_bits_per_sample_t(BITS_PER_SAMPLE_ADC),
                .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT, //I2S_CHANNEL_FMT_ONLY_LEFT //I2S_CHANNEL_FMT_ALL_RIGHT
                .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S), // If fails to compile try changing this line
                .intr_alloc_flags = ESP_INTR_FLAG_LEVEL2, // The priority of the interupt
                .dma_buf_count = 8, //2
                .dma_buf_len = BUFFER_LEN_ADC,
                .use_apll = true, //  APLL generate highly accurate clocks
                .tx_desc_auto_clear = false // Auto clear transmission buffer
            };
            if(i2s_driver_install(I2S_PORT_ADC, &i2s_config, 0, NULL) != ESP_OK){
                Serial.println("ERROR in initializing ADC driver.");
            }
        }

        static void i2s_setpin_mic() {
            // Set I2S pin configuration
            const i2s_pin_config_t pin_config = {
              .bck_io_num = I2S_SCK,
              .ws_io_num = I2S_WS,
              .data_out_num = -1,
              .data_in_num = I2S_SD_MIC
            };
            i2s_set_pin(I2S_PORT_MIC, &pin_config);
        }

        static void i2s_setpin_adc() {
            // Set I2S pin configuration
            const i2s_pin_config_t pin_config = {
              .bck_io_num = I2S_SCK,
              .ws_io_num = I2S_WS,
              .data_out_num = -1,
              .data_in_num = I2S_SD_ADC
            };
            i2s_set_pin(I2S_PORT_ADC, &pin_config);
        }

        static void switchAudioInput(uint8_t periferalCode){
            switch (periferalCode)
            {
            case MODE_ADC_FFT_ON:
                i2s_stop(I2S_PORT_MIC);
                i2s_start(I2S_PORT_ADC);
                //i2s_adc_install();
                i2s_setpin_adc();
                Serial.println("start ADC");
                Audio::currentPort = I2S_PORT_ADC;
                Audio::currentBufferLength = BUFFER_LEN_ADC;
                break;
            case MODE_MIC_FFT_ON:
                i2s_stop(I2S_PORT_ADC);
                i2s_start(I2S_PORT_MIC);
                //i2s_mic_install();
                i2s_setpin_mic();
                Serial.println("start MIC");
                Audio::currentPort = I2S_PORT_MIC;
                Audio::currentBufferLength = BUFFER_LEN_MIC;
                break;
            case MODE_FFT_OFF:
                i2s_stop(I2S_PORT_MIC);
                i2s_stop(I2S_PORT_ADC);
                break;
            default:
                break;
            }
            
        }

        //static void switchPortsOff(){
        //    i2s_stop(I2S_PORT_MIC);
        //    i2s_stop(I2S_PORT_ADC);
        //}

        static void setup() {
            i2s_adc_install();
            i2s_setpin_adc();
            i2s_mic_install();
            i2s_setpin_mic();
            //i2s_stop(I2S_PORT_MIC);
            //i2s_stop(I2S_PORT_ADC);
        }

        template<typename T> static size_t readAudio(void* buffer){
            // Get I2S data and place in data buffer
            size_t bytesIn = 0;
            i2s_read(Audio::currentPort, buffer, Audio::currentBufferLength * sizeof(T), &bytesIn, portMAX_DELAY);
            return bytesIn;
        }

};

//i2s_port_t Audio::currentPort = I2S_PORT_ADC;
//
//size_t Audio::currentBufferLength = BUFFER_LEN_ADC;
/*
void Audio::i2s_mic_install() {
    // Set up I2S Processor configuration
    const i2s_config_t i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE_MIC,
        .bits_per_sample = i2s_bits_per_sample_t(BITS_PER_SAMPLE_MIC),
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT, //I2S_CHANNEL_FMT_ALL_RIGHT
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S), // If fails to compile try changing this line
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL2, //ESP_INTR_FLAG_LEVEL1
        .dma_buf_count = 8, //2
        .dma_buf_len = BUFFER_LEN_MIC,
        .use_apll = false
    };
    i2s_driver_install(I2S_PORT_MIC, &i2s_config, 0, NULL);
}*/
/*
void Audio::i2s_adc_install() {
    // Set up I2S Processor configuration
    const i2s_config_t i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE_ADC,
        .bits_per_sample = i2s_bits_per_sample_t(BITS_PER_SAMPLE_ADC),
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT, //I2S_CHANNEL_FMT_ALL_RIGHT
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S), // If fails to compile try changing this line
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL2,
        .dma_buf_count = 8, //2
        .dma_buf_len = BUFFER_LEN_ADC,
        .use_apll = false
    };
    i2s_driver_install(I2S_PORT_ADC, &i2s_config, 0, NULL);
}
*/
/*
void Audio::i2s_setpin_mic() {
    // Set I2S pin configuration
    const i2s_pin_config_t pin_config = {
      .bck_io_num = I2S_SCK,
      .ws_io_num = I2S_WS,
      .data_out_num = -1,
      .data_in_num = I2S_SD_MIC
    };
    i2s_set_pin(I2S_PORT_MIC, &pin_config);
}
*/
/*
void Audio::i2s_setpin_adc() {
    // Set I2S pin configuration
    const i2s_pin_config_t pin_config = {
      .bck_io_num = I2S_SCK,
      .ws_io_num = I2S_WS,
      .data_out_num = -1,
      .data_in_num = I2S_SD_ADC
    };
    i2s_set_pin(I2S_PORT_ADC, &pin_config);
}
*/
/*
void Audio::setup() {
    i2s_mic_install();
    i2s_setpin_mic();
    i2s_adc_install();
    i2s_setpin_adc();
    i2s_stop(I2S_PORT_MIC);
    i2s_stop(I2S_PORT_ADC);
}
*/
/*
void Audio::switchAudioInput(uint8_t periferalCode){

}
*/
#endif