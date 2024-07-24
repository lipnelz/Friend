#ifndef __CODEC_H
#define __CODEC_H

#ifdef __cplusplus
 extern "C" {
#endif


#include <zephyr/kernel.h>

// Callback
typedef void (*codec_callback)(uint8_t *data, size_t len);
void set_codec_callback(codec_callback callback);

// Integration
int codec_receive_pcm(int16_t *data, size_t len);
int codec_start();

#ifdef __cplusplus
}
#endif

#endif /* __CODEC_H */