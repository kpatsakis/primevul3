int ff_mpeg4_decode_studio_slice_header(Mpeg4DecContext *ctx)
{
    MpegEncContext *s = &ctx->m;
    GetBitContext *gb = &s->gb;
    unsigned vlc_len;
    uint16_t mb_num;

    if (get_bits_left(gb) >= 32 && get_bits_long(gb, 32) == SLICE_START_CODE) {
        vlc_len = av_log2(s->mb_width * s->mb_height) + 1;
        mb_num = get_bits(gb, vlc_len);

        if (mb_num >= s->mb_num)
            return AVERROR_INVALIDDATA;

        s->mb_x = mb_num % s->mb_width;
        s->mb_y = mb_num / s->mb_width;

        if (ctx->shape != BIN_ONLY_SHAPE)
            s->qscale = mpeg_get_qscale(s);

        if (get_bits1(gb)) {  /* slice_extension_flag */
            skip_bits1(gb);   /* intra_slice */
            skip_bits1(gb);   /* slice_VOP_id_enable */
            skip_bits(gb, 6); /* slice_VOP_id */
            while (get_bits1(gb)) /* extra_bit_slice */
                skip_bits(gb, 8); /* extra_information_slice */
        }

        reset_studio_dc_predictors(s);
    }
    else {
        return AVERROR_INVALIDDATA;
    }

    return 0;
}
