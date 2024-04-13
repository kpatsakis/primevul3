void ass_stripe_unpack_c(int16_t *dst, const uint8_t *src, ptrdiff_t src_stride,
                         uintptr_t width, uintptr_t height)
{
    for (uintptr_t y = 0; y < height; ++y) {
        int16_t *ptr = dst;
        for (uintptr_t x = 0; x < width; x += STRIPE_WIDTH) {
            for (int k = 0; k < STRIPE_WIDTH; ++k)
                ptr[k] = (uint16_t)(((src[x + k] << 7) | (src[x + k] >> 1)) + 1) >> 1;
            ptr += STRIPE_WIDTH * height;
        }
        dst += STRIPE_WIDTH;
        src += src_stride;
    }
}
