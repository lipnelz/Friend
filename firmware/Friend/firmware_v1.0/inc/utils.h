#ifndef __UTILS_H
#define __UTILS_H

#ifdef __cplusplus
 extern "C" {
#endif


#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/gatt.h>
#include "config.h"
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/gatt.h>


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
