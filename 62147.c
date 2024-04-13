void ass_blur1234_horz_c(int16_t *dst, const int16_t *src,
                         uintptr_t src_width, uintptr_t src_height,
                         const int16_t *param)
{
    uintptr_t dst_width = src_width + 8;
    uintptr_t size = ((src_width + STRIPE_MASK) & ~STRIPE_MASK) * src_height;
    uintptr_t step = STRIPE_WIDTH * src_height;

    uintptr_t offs = 0;
    int16_t buf[2 * STRIPE_WIDTH];
    int16_t *ptr = buf + STRIPE_WIDTH;
    for (uintptr_t x = 0; x < dst_width; x += STRIPE_WIDTH) {
        for (uintptr_t y = 0; y < src_height; ++y) {
            copy_line(ptr - 1 * STRIPE_WIDTH, src, offs - 1 * step, size);
            copy_line(ptr - 0 * STRIPE_WIDTH, src, offs - 0 * step, size);
            for (int k = 0; k < STRIPE_WIDTH; ++k)
                dst[k] = blur_func(ptr[k - 8], ptr[k - 7], ptr[k - 6], ptr[k - 5], ptr[k - 4],
                                   ptr[k - 3], ptr[k - 2], ptr[k - 1], ptr[k - 0], param);
            dst += STRIPE_WIDTH;
            offs += STRIPE_WIDTH;
        }
    }
}
