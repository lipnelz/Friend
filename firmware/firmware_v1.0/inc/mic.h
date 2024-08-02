<<<<<<< HEAD
#ifndef __MIC_H
#define __MIC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "utils.h"
#include "friend.h"

int mic_start(void);
void set_mic_callback(Friend_Ctx_s *ctx, mix_handler callback);

#ifdef __cplusplus
}
#endif

#endif /* __MIC_H */
=======
#pragma once

typedef void (*mix_handler)(int16_t *);
int mic_start();
void set_mic_callback(mix_handler _callback);
>>>>>>> 502e2fcc (fixup! fix to transcribe endpoint (removed speech profiles))
