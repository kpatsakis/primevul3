static void ff_jref_idct1_put(uint8_t *dest, ptrdiff_t line_size, int16_t *block)
{
    dest[0] = av_clip_uint8((block[0] + 4)>>3);
}
