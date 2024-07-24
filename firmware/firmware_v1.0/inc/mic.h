#ifndef __MIC_H
#define __MIC_H

#ifdef __cplusplus
 extern "C" {
#endif


typedef void (*mix_handler)(int16_t *);
int mic_start();
void set_mic_callback(mix_handler _callback);


#ifdef __cplusplus
}
#endif

#endif /* __MIC_H */
