#ifndef __AUDIO_H
#define __AUDIO_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

uint8_t linear2ulaw(int pcm_val);


#ifdef __cplusplus
}
#endif

#endif /* __AUDIO_H */