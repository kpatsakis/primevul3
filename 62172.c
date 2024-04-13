static inline int16_t pre_blur3_func(int16_t p3, int16_t p2, int16_t p1, int16_t z0,
                                     int16_t n1, int16_t n2, int16_t n3)
{
    /*
    return (1 * p3 + 6 * p2 + 15 * p1 + 20 * z0 + 15 * n1 + 6 * n2 + 1 * n3 + 32) >> 6;
    */
    return (20 * (uint16_t)z0 +
            15 * (uint16_t)(p1 + n1) +
             6 * (uint16_t)(p2 + n2) +
             1 * (uint16_t)(p3 + n3) + 32) >> 6;
}
