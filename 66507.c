static void ff_jref_idct1_add(uint8_t *dest, ptrdiff_t line_size, int16_t *block)
{
    dest[0] = av_clip_uint8(dest[0] + ((block[0] + 4)>>3));
}
