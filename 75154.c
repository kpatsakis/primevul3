static inline unsigned fastLog2(unsigned i)
{
    unsigned log2 = 0;
    if (i & (i - 1))
        log2 += 1;
    if (i >> 16)
        log2 += 16, i >>= 16;
    if (i >> 8)
        log2 += 8, i >>= 8;
    if (i >> 4)
        log2 += 4, i >>= 4;
    if (i >> 2)
        log2 += 2, i >>= 2;
    if (i >> 1)
        log2 += 1;
    return log2;
}
