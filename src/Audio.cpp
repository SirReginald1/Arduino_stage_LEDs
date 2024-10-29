#include "Audio.h"

i2s_port_t Audio::currentPort = I2S_PORT_ADC;

size_t Audio::currentBufferLength = BUFFER_LEN_ADC;