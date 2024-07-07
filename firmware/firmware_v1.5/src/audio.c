#include <stdio.h>


#define BIAS (0x84U) /* Bias for linear code. */
#define SEG_SIZE (0x08U)

uint8_t linear2ulaw(__int32_t pcm_val) /* 2's complement (16-bit range) */
{
    static const uint16_t seg_end[SEG_SIZE] = {0x001F, 0x003F, 0x007F, 0x00FF, 0x01FF, 0x03FF, 0x07FF, 0x0FFF};
    __int32_t mask = 0;
    uint8_t uval = 0;
    uint8_t idx = 0;

    /* Get the sign and the magnitude of the value. */
    if (pcm_val < 0)
    {
        pcm_val = BIAS - pcm_val;
        mask = 0x7F;
    }
    else
    {
        pcm_val += BIAS;
        mask = 0xFF;
    }

    /* Convert the scaled magnitude to segment number. */
     for (idx = 0; idx < SEG_SIZE; idx++)
     {
        if (pcm_val <= seg_end[idx])
            break;
    }

    /*
     * Combine the sign, segment, quantization bits;
     * and complement the code word.
     */
    if (seg_end[idx] >= 8) /* out of range, return maximum value. */
    {
        return (0x7F ^ mask);
    }

    uval = (uint8_t)(seg_end[idx] << 0x04) | ((pcm_val >> (seg_end[idx] + 0x03)) & 0x0F);
    return (uval ^ mask);
}
