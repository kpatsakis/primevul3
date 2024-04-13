static void ff_jref_idct2_put(uint8_t *dest, ptrdiff_t line_size, int16_t *block)
{
    ff_j_rev_dct2 (block);
    put_pixels_clamped2_c(block, dest, line_size);
}
