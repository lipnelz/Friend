<<<<<<< HEAD
#ifndef __CODEC_H
#define __CODEC_H

#ifdef __cplusplus
 extern "C" {
#endif


=======
#pragma once
>>>>>>> 502e2fcc (fixup! fix to transcribe endpoint (removed speech profiles))
#include <zephyr/kernel.h>

// Callback
typedef void (*codec_callback)(uint8_t *data, size_t len);
void set_codec_callback(codec_callback callback);

// Integration
int codec_receive_pcm(int16_t *data, size_t len);
<<<<<<< HEAD
int codec_start();

#ifdef __cplusplus
}
#endif

#endif /* __CODEC_H */
=======
int codec_start();
>>>>>>> 502e2fcc (fixup! fix to transcribe endpoint (removed speech profiles))
