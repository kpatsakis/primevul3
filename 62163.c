static inline int16_t blur_func(int16_t p4, int16_t p3, int16_t p2, int16_t p1, int16_t z0,
                                int16_t n1, int16_t n2, int16_t n3, int16_t n4, const int16_t c[])
{
    p1 -= z0;
    p2 -= z0;
    p3 -= z0;
    p4 -= z0;
    n1 -= z0;
    n2 -= z0;
    n3 -= z0;
    n4 -= z0;
    return (((p1 + n1) * c[0] +
             (p2 + n2) * c[1] +
             (p3 + n3) * c[2] +
             (p4 + n4) * c[3] +
             0x8000) >> 16) + z0;
}
