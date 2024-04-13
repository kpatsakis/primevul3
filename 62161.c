void ass_shrink_vert_c(int16_t *dst, const int16_t *src,
                       uintptr_t src_width, uintptr_t src_height)
{
    uintptr_t dst_height = (src_height + 5) >> 1;
    uintptr_t step = STRIPE_WIDTH * src_height;

    for (uintptr_t x = 0; x < src_width; x += STRIPE_WIDTH) {
        uintptr_t offs = 0;
        for (uintptr_t y = 0; y < dst_height; ++y) {
            const int16_t *p1p = get_line(src, offs - 4 * STRIPE_WIDTH, step);
            const int16_t *p1n = get_line(src, offs - 3 * STRIPE_WIDTH, step);
            const int16_t *z0p = get_line(src, offs - 2 * STRIPE_WIDTH, step);
            const int16_t *z0n = get_line(src, offs - 1 * STRIPE_WIDTH, step);
            const int16_t *n1p = get_line(src, offs - 0 * STRIPE_WIDTH, step);
            const int16_t *n1n = get_line(src, offs + 1 * STRIPE_WIDTH, step);
            for (int k = 0; k < STRIPE_WIDTH; ++k)
                dst[k] = shrink_func(p1p[k], p1n[k], z0p[k], z0n[k], n1p[k], n1n[k]);
            dst += STRIPE_WIDTH;
            offs += 2 * STRIPE_WIDTH;
        }
        src += step;
    }
}
