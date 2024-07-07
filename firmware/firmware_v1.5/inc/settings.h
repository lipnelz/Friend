#ifndef __SETTINGS_H
#define __SETTINGS_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <zephyr/kernel.h>
#include "utils.h"



int settings_start(void);
bool settings_read_enable(Friend_ctx_s *ctx);
int settings_write_enable(Friend_ctx_s *ctx);


#ifdef __cplusplus
}
#endif

#endif /* __SETTINGS_H */
