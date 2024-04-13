void ass_blur1234_vert_c(int16_t *dst, const int16_t *src,
                         uintptr_t src_width, uintptr_t src_height,
                         const int16_t *param)
{
    uintptr_t dst_height = src_height + 8;
    uintptr_t step = STRIPE_WIDTH * src_height;

    for (uintptr_t x = 0; x < src_width; x += STRIPE_WIDTH) {
        uintptr_t offs = 0;
        for (uintptr_t y = 0; y < dst_height; ++y) {
            const int16_t *p4 = get_line(src, offs - 8 * STRIPE_WIDTH, step);
            const int16_t *p3 = get_line(src, offs - 7 * STRIPE_WIDTH, step);
            const int16_t *p2 = get_line(src, offs - 6 * STRIPE_WIDTH, step);
            const int16_t *p1 = get_line(src, offs - 5 * STRIPE_WIDTH, step);
            const int16_t *z0 = get_line(src, offs - 4 * STRIPE_WIDTH, step);
            const int16_t *n1 = get_line(src, offs - 3 * STRIPE_WIDTH, step);
            const int16_t *n2 = get_line(src, offs - 2 * STRIPE_WIDTH, step);
            const int16_t *n3 = get_line(src, offs - 1 * STRIPE_WIDTH, step);
            const int16_t *n4 = get_line(src, offs - 0 * STRIPE_WIDTH, step);
            for (int k = 0; k < STRIPE_WIDTH; ++k)
                dst[k] = blur_func(p4[k], p3[k], p2[k], p1[k], z0[k],
                                   n1[k], n2[k], n3[k], n4[k], param);
            dst += STRIPE_WIDTH;
            offs += STRIPE_WIDTH;
        }
        src += step;
    }
}
