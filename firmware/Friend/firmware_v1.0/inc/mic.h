#ifndef __MIC_H
#define __MIC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "utils.h"
<<<<<<< HEAD
#include "friend.h"
=======
>>>>>>> 35823aad (clean headers)

int mic_start(void);
void set_mic_callback(Friend_Ctx_s *ctx, mix_handler callback);

#ifdef __cplusplus
}
#endif

#endif /* __MIC_H */