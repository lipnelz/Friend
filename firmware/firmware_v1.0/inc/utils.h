<<<<<<< HEAD
#ifndef __UTILS_H
#define __UTILS_H

#ifdef __cplusplus
 extern "C" {
#endif


#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/gatt.h>
#include "config.h"
=======
#pragma once
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/gatt.h>
>>>>>>> 502e2fcc (fixup! fix to transcribe endpoint (removed speech profiles))

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

<<<<<<< HEAD

#ifdef __cplusplus
}
#endif

#endif /* __UTILS_H */
=======
// #define WAIT_LOG k_sleep(K_MSEC(200));
// #define WAIT_LOG do {} while(0);
// #define WAIT_LOG z_impl_log_process()
// #define WAIT_LOG while (z_impl_log_process() == true) { }
>>>>>>> 502e2fcc (fixup! fix to transcribe endpoint (removed speech profiles))
