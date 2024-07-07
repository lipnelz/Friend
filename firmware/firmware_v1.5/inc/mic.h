#ifndef __MIC_H
#define __MIC_H

#ifdef __cplusplus
 extern "C" {
#endif


int mic_start(void);
int mic_resume(void);
int mic_pause(void);

typedef void (*mix_handler)(int16_t *);
void set_mic_callback(mix_handler _callback);


#ifdef __cplusplus
}
#endif

#endif /* __MIC_H */

