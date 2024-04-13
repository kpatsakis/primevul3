void ass_pre_blur1_vert_c(int16_t *dst, const int16_t *src,
                          uintptr_t src_width, uintptr_t src_height)
{
    uintptr_t dst_height = src_height + 2;
    uintptr_t step = STRIPE_WIDTH * src_height;

    for (uintptr_t x = 0; x < src_width; x += STRIPE_WIDTH) {
        uintptr_t offs = 0;
        for (uintptr_t y = 0; y < dst_height; ++y) {
            const int16_t *p1 = get_line(src, offs - 2 * STRIPE_WIDTH, step);
            const int16_t *z0 = get_line(src, offs - 1 * STRIPE_WIDTH, step);
            const int16_t *n1 = get_line(src, offs - 0 * STRIPE_WIDTH, step);
            for (int k = 0; k < STRIPE_WIDTH; ++k)
                dst[k] = pre_blur1_func(p1[k], z0[k], n1[k]);
            dst += STRIPE_WIDTH;
            offs += STRIPE_WIDTH;
        }
        src += step;
    }
}
