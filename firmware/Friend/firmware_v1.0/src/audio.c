#include <stdint.h>

#define BIAS (0x84) /* Bias for linear code. */

static int16_t seg_end[8] = { 0x00FF, 0x01FF, 0x03FF, 0x07FF, 0x0FFF, 0x1FFF, 0x3FFF, 0x7FFF };


static int search(int val, int16_t* table, int size);

static int search(int val, int16_t* table, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (val <= *table++)
            return (i);
    }
    return (size);
}

uint8_t linear2ulaw(int pcm_val) /* 2's complement (16-bit range) */
{
    int seg;
    uint8_t mask;
    uint8_t uval;

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
    seg = search(pcm_val, seg_end, 8);

    /*
     * Combine the sign, segment, quantization bits;
     * and complement the code word.
     */
    if (seg >= 8) /* out of range, return maximum value. */
    {
        return (0x7F ^ mask);
    }
    uval = (seg << 4) | ((pcm_val >> (seg + 3)) & 0xF);
    return (uval ^ mask);
}