static void ff_jref_idct4_add(uint8_t *dest, ptrdiff_t line_size, int16_t *block)
{
    ff_j_rev_dct4 (block);
    add_pixels_clamped4_c(block, dest, line_size);
}
