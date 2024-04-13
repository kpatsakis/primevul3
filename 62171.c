static inline int16_t pre_blur2_func(int16_t p2, int16_t p1, int16_t z0,
                                     int16_t n1, int16_t n2)
{
    /*
    return (1 * p2 + 4 * p1 + 6 * z0 + 4 * n1 + 1 * n2 + 8) >> 4;
    */
    uint16_t r1 = ((uint16_t)(((uint16_t)(p2 + n2) >> 1) + z0) >> 1) + z0;
    uint16_t r2 = p1 + n1;
    uint16_t r = ((uint16_t)(r1 + r2) >> 1) | (0x8000 & r1 & r2);
    return (uint16_t)(r + 1) >> 1;
}
