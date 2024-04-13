static uint32_t __inline read_code (Bitstream *bs, uint32_t maxcode)
{
    unsigned long local_sr;
    uint32_t extras, code;
    int bitcount;

    if (maxcode < 2)
        return maxcode ? getbit (bs) : 0;

    bitcount = count_bits (maxcode);
#ifdef USE_BITMASK_TABLES
    extras = bitset [bitcount] - maxcode - 1;
#else
    extras = (1 << bitcount) - maxcode - 1;
#endif

    local_sr = bs->sr;

    while (bs->bc < bitcount) {
        if (++(bs->ptr) == bs->end)
            bs->wrap (bs);

        local_sr |= (long)*(bs->ptr) << bs->bc;
        bs->bc += sizeof (*(bs->ptr)) * 8;
    }

#ifdef USE_BITMASK_TABLES
    if ((code = local_sr & bitmask [bitcount - 1]) >= extras)
#else
    if ((code = local_sr & ((1 << (bitcount - 1)) - 1)) >= extras)
#endif
        code = (code << 1) - extras + ((local_sr >> (bitcount - 1)) & 1);
    else
        bitcount--;

    if (sizeof (local_sr) < 8 && bs->bc > sizeof (local_sr) * 8) {
        bs->bc -= bitcount;
        bs->sr = *(bs->ptr) >> (sizeof (*(bs->ptr)) * 8 - bs->bc);
    }
    else {
        bs->bc -= bitcount;
        bs->sr = local_sr >> bitcount;
    }

    return code;
}
