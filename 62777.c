static int mpeg4_decode_studio_mb(MpegEncContext *s, int16_t block_[12][64])
{
    int i;

    /* StudioMacroblock */
    /* Assumes I-VOP */
    s->mb_intra = 1;
    if (get_bits1(&s->gb)) { /* compression_mode */
        /* DCT */
        /* macroblock_type, 1 or 2-bit VLC */
        if (!get_bits1(&s->gb)) {
            skip_bits1(&s->gb);
            s->qscale = mpeg_get_qscale(s);
        }

        for (i = 0; i < mpeg4_block_count[s->chroma_format]; i++) {
            if (mpeg4_decode_studio_block(s, (*s->block32)[i], i) < 0)
                return AVERROR_INVALIDDATA;
        }
    } else {
        /* DPCM */
        check_marker(s->avctx, &s->gb, "DPCM block start");
        avpriv_request_sample(s->avctx, "DPCM encoded block");
        next_start_code_studio(&s->gb);
        return SLICE_ERROR;
    }

    if (get_bits_left(&s->gb) >= 24 && show_bits(&s->gb, 23) == 0) {
        next_start_code_studio(&s->gb);
        return SLICE_END;
    }

    return SLICE_OK;
}
