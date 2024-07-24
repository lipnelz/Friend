#include "friend.h"

Friend_Ctx_s Friend_ctx;


void init_friend_context(Friend_Ctx_s *ctx)
{
  ctx->is_charging = false;
  ctx->is_connected = false;

  ctx->mic._callback = NULL;
  ctx->mic._next_buffer_index = 0;
  memset(ctx->mic._buffer_0, 0, MIC_BUFFER_SAMPLES);
  memset(ctx->mic._buffer_1, 0, MIC_BUFFER_SAMPLES);
}

uint8_t get_friend_mic_buff_idx(void)
{
  return Friend_ctx.mic._next_buffer_index;
}

void set_friend_mic_buff_idx(uint8_t value)
{
  Friend_ctx.mic._next_buffer_index = value;
}

void set_friend_connection_state(bool state)
{
  Friend_ctx.is_connected = state;
}

int16_t* get_friend_buffer(uint8_t index)
{
  if(index == 0)
  {
    return Friend_ctx.mic._buffer_0;
  }
  return Friend_ctx.mic._buffer_1;
}

void call_friend_mic_cb(int16_t *buf_rel)
{
  if (Friend_ctx.mic._callback)
  {
    Friend_ctx.mic._callback(buf_rel);
  }
}
