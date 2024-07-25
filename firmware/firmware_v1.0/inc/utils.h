#ifndef __UTILS_H
#define __UTILS_H

#ifdef __cplusplus
 extern "C" {
#endif


#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/gatt.h>
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

Friend_Ctx_s* get_friend_context(void);


#define ASSERT_OK(result)                                          \
    if ((result) < 0)                                              \
    {                                                              \
        printk("Error at %s:%d:%d\n", __FILE__, __LINE__, result); \
        return (result);                                           \
    }

#define ASSERT_TRUE(result)                                        \
    if (!result)                                                   \
    {                                                              \
        printk("Error at %s:%d:%d\n", __FILE__, __LINE__, result); \
        return -1;                                                 \
    }


#ifdef __cplusplus
}
#endif

#endif /* __UTILS_H */