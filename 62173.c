static inline int16_t shrink_func(int16_t p1p, int16_t p1n,
                                  int16_t z0p, int16_t z0n,
                                  int16_t n1p, int16_t n1n)
{
    /*
    return (1 * p1p + 5 * p1n + 10 * z0p + 10 * z0n + 5 * n1p + 1 * n1n + 16) >> 5;
    */
    int32_t r = (p1p + p1n + n1p + n1n) >> 1;
    r = (r + z0p + z0n) >> 1;
    r = (r + p1n + n1p) >> 1;
    return (r + z0p + z0n + 2) >> 2;
}
