int av_ac3_parse_header(const uint8_t *buf, size_t size,
                        uint8_t *bitstream_id, uint16_t *frame_size)
{
    GetBitContext gb;
    AC3HeaderInfo hdr;
    int err;

    init_get_bits8(&gb, buf, size);
    err = ff_ac3_parse_header(&gb, &hdr);
    if (err < 0)
        return AVERROR_INVALIDDATA;

    *bitstream_id = hdr.bitstream_id;
    *frame_size   = hdr.frame_size;

    return 0;
}
