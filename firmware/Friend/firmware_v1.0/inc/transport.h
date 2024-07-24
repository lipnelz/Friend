#ifndef __TRANSPORT_H
#define __TRANSPORT_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <zephyr/kernel.h>

int transport_start(void);
int broadcast_audio_packets(uint8_t *buffer, size_t size);


#ifdef __cplusplus
}
#endif

#endif /* __TRANSPORT_H */
