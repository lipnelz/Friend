<<<<<<< HEAD
#ifndef __TRANSPORT_H
#define __TRANSPORT_H

#ifdef __cplusplus
 extern "C" {
#endif


#include <zephyr/kernel.h>
int transport_start();
int broadcast_audio_packets(uint8_t *buffer, size_t size);


#ifdef __cplusplus
}
#endif

#endif /* __TRANSPORT_H */
=======
#pragma once
#include <zephyr/kernel.h>
int transport_start();
int broadcast_audio_packets(uint8_t *buffer, size_t size);
>>>>>>> 502e2fcc (fixup! fix to transcribe endpoint (removed speech profiles))
