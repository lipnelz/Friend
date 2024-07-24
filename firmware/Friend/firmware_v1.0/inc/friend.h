#ifndef __FRIEND_H
#define __FRIEND_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include "config.h"

typedef void (*mix_handler)(int16_t *);

typedef struct Mic_context
{
    int16_t _buffer_0[MIC_BUFFER_SAMPLES];
    int16_t _buffer_1[MIC_BUFFER_SAMPLES];
    volatile uint8_t _next_buffer_index;
    volatile mix_handler _callback;
} Mic_ctx_s;

typedef struct Friend_context
{
  bool is_connected;
  bool is_charging;
  Mic_ctx_s mic;
} Friend_Ctx_s;

void init_friend_context(Friend_Ctx_s *ctx);
void set_friend_connection_state(bool state);
uint8_t get_friend_mic_buff_idx(void);
void set_friend_mic_buff_idx(uint8_t value);
int16_t* get_friend_buffer(uint8_t index);
void call_friend_mic_cb(int16_t *buf_rel);


#ifdef __cplusplus
}
#endif

#endif /* __FRIEND_H */