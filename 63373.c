static int block_add_checksum (unsigned char *buffer_start, unsigned char *buffer_end, int bytes)
{
    WavpackHeader *wphdr = (WavpackHeader *) buffer_start;
#ifdef BITSTREAM_SHORTS
    uint16_t *csptr = (uint16_t*) buffer_start;
#else
    unsigned char *csptr = buffer_start;
#endif
    int bcount = wphdr->ckSize + 8, wcount;
    uint32_t csum = (uint32_t) -1;

    if (bytes != 2 && bytes != 4)
        return FALSE;

    if (bcount < sizeof (WavpackHeader) || (bcount & 1) || buffer_start + bcount + 2 + bytes > buffer_end)
        return FALSE;

    wphdr->flags |= HAS_CHECKSUM;
    wphdr->ckSize += 2 + bytes;
    wcount = bcount >> 1;

#ifdef BITSTREAM_SHORTS
    while (wcount--)
        csum = (csum * 3) + *csptr++;
#else
    WavpackNativeToLittleEndian ((WavpackHeader *) buffer_start, WavpackHeaderFormat);

    while (wcount--) {
        csum = (csum * 3) + csptr [0] + (csptr [1] << 8);
        csptr += 2;
    }

    WavpackLittleEndianToNative ((WavpackHeader *) buffer_start, WavpackHeaderFormat);
#endif

    buffer_start += bcount;
    *buffer_start++ = ID_BLOCK_CHECKSUM;
    *buffer_start++ = bytes >> 1;

    if (bytes == 4) {
        *buffer_start++ = csum;
        *buffer_start++ = csum >> 8;
        *buffer_start++ = csum >> 16;
        *buffer_start++ = csum >> 24;
    }
    else {
        csum ^= csum >> 16;
        *buffer_start++ = csum;
        *buffer_start++ = csum >> 8;
    }

    return TRUE;
}
