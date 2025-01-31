static int decode_slice(MpegEncContext *s)
{
    const int part_mask = s->partitioned_frame
                          ? (ER_AC_END | ER_AC_ERROR) : 0x7F;
    const int mb_size   = 16 >> s->avctx->lowres;
    int ret;

    s->last_resync_gb   = s->gb;
    s->first_slice_line = 1;
    s->resync_mb_x      = s->mb_x;
    s->resync_mb_y      = s->mb_y;

    ff_set_qscale(s, s->qscale);

    if (s->studio_profile) {
        if ((ret = ff_mpeg4_decode_studio_slice_header(s->avctx->priv_data)) < 0)
            return ret;
    }

    if (s->avctx->hwaccel) {
        const uint8_t *start = s->gb.buffer + get_bits_count(&s->gb) / 8;
        ret = s->avctx->hwaccel->decode_slice(s->avctx, start, s->gb.buffer_end - start);
        s->mb_y = s->mb_height;
        return ret;
    }

    if (s->partitioned_frame) {
        const int qscale = s->qscale;

        if (CONFIG_MPEG4_DECODER && s->codec_id == AV_CODEC_ID_MPEG4)
            if ((ret = ff_mpeg4_decode_partitions(s->avctx->priv_data)) < 0)
                return ret;

        /* restore variables which were modified */
        s->first_slice_line = 1;
        s->mb_x             = s->resync_mb_x;
        s->mb_y             = s->resync_mb_y;
        ff_set_qscale(s, qscale);
    }

    for (; s->mb_y < s->mb_height; s->mb_y++) {
        /* per-row end of slice checks */
        if (s->msmpeg4_version) {
            if (s->resync_mb_y + s->slice_height == s->mb_y) {
                ff_er_add_slice(&s->er, s->resync_mb_x, s->resync_mb_y,
                                s->mb_x - 1, s->mb_y, ER_MB_END);

                return 0;
            }
        }

        if (s->msmpeg4_version == 1) {
            s->last_dc[0] =
            s->last_dc[1] =
            s->last_dc[2] = 128;
        }

        ff_init_block_index(s);
        for (; s->mb_x < s->mb_width; s->mb_x++) {
            int ret;

            ff_update_block_index(s);

            if (s->resync_mb_x == s->mb_x && s->resync_mb_y + 1 == s->mb_y)
                s->first_slice_line = 0;

            /* DCT & quantize */

            s->mv_dir  = MV_DIR_FORWARD;
            s->mv_type = MV_TYPE_16X16;
            ff_dlog(s, "%d %06X\n",
                    get_bits_count(&s->gb), show_bits(&s->gb, 24));

            ff_tlog(NULL, "Decoding MB at %dx%d\n", s->mb_x, s->mb_y);
            ret = s->decode_mb(s, s->block);

            if (s->pict_type != AV_PICTURE_TYPE_B)
                ff_h263_update_motion_val(s);

            if (ret < 0) {
                const int xy = s->mb_x + s->mb_y * s->mb_stride;
                if (ret == SLICE_END) {
                    ff_mpv_reconstruct_mb(s, s->block);
                    if (s->loop_filter)
                        ff_h263_loop_filter(s);

                    ff_er_add_slice(&s->er, s->resync_mb_x, s->resync_mb_y,
                                    s->mb_x, s->mb_y, ER_MB_END & part_mask);

                    s->padding_bug_score--;

                    if (++s->mb_x >= s->mb_width) {
                        s->mb_x = 0;
                        ff_mpeg_draw_horiz_band(s, s->mb_y * mb_size, mb_size);
                        ff_mpv_report_decode_progress(s);
                        s->mb_y++;
                    }
                    return 0;
                } else if (ret == SLICE_NOEND) {
                    av_log(s->avctx, AV_LOG_ERROR,
                           "Slice mismatch at MB: %d\n", xy);
                    ff_er_add_slice(&s->er, s->resync_mb_x, s->resync_mb_y,
                                    s->mb_x + 1, s->mb_y,
                                    ER_MB_END & part_mask);
                    return AVERROR_INVALIDDATA;
                }
                av_log(s->avctx, AV_LOG_ERROR, "Error at MB: %d\n", xy);
                ff_er_add_slice(&s->er, s->resync_mb_x, s->resync_mb_y,
                                s->mb_x, s->mb_y, ER_MB_ERROR & part_mask);

                if (s->avctx->err_recognition & AV_EF_IGNORE_ERR)
                    continue;
                return AVERROR_INVALIDDATA;
            }

            ff_mpv_reconstruct_mb(s, s->block);
            if (s->loop_filter)
                ff_h263_loop_filter(s);
        }

        ff_mpeg_draw_horiz_band(s, s->mb_y * mb_size, mb_size);
        ff_mpv_report_decode_progress(s);

        s->mb_x = 0;
    }

    av_assert1(s->mb_x == 0 && s->mb_y == s->mb_height);

    if (s->codec_id == AV_CODEC_ID_MPEG4         &&
        (s->workaround_bugs & FF_BUG_AUTODETECT) &&
        get_bits_left(&s->gb) >= 48              &&
        show_bits(&s->gb, 24) == 0x4010          &&
        !s->data_partitioning)
        s->padding_bug_score += 32;

    /* try to detect the padding bug */
    if (s->codec_id == AV_CODEC_ID_MPEG4         &&
        (s->workaround_bugs & FF_BUG_AUTODETECT) &&
        get_bits_left(&s->gb) >= 0               &&
        get_bits_left(&s->gb) < 137              &&
        !s->data_partitioning) {
        const int bits_count = get_bits_count(&s->gb);
        const int bits_left  = s->gb.size_in_bits - bits_count;

        if (bits_left == 0) {
            s->padding_bug_score += 16;
        } else if (bits_left != 1) {
            int v = show_bits(&s->gb, 8);
            v |= 0x7F >> (7 - (bits_count & 7));

            if (v == 0x7F && bits_left <= 8)
                s->padding_bug_score--;
            else if (v == 0x7F && ((get_bits_count(&s->gb) + 8) & 8) &&
                     bits_left <= 16)
                s->padding_bug_score += 4;
            else
                s->padding_bug_score++;
        }
    }

    if (s->codec_id == AV_CODEC_ID_H263          &&
        (s->workaround_bugs & FF_BUG_AUTODETECT) &&
        get_bits_left(&s->gb) >= 8               &&
        get_bits_left(&s->gb) < 300              &&
        s->pict_type == AV_PICTURE_TYPE_I        &&
        show_bits(&s->gb, 8) == 0                &&
        !s->data_partitioning) {

        s->padding_bug_score += 32;
    }

    if (s->codec_id == AV_CODEC_ID_H263          &&
        (s->workaround_bugs & FF_BUG_AUTODETECT) &&
        get_bits_left(&s->gb) >= 64              &&
        AV_RB64(s->gb.buffer_end - 8) == 0xCDCDCDCDFC7F0000) {

        s->padding_bug_score += 32;
    }

    if (s->workaround_bugs & FF_BUG_AUTODETECT) {
        if (
            (s->padding_bug_score > -2 && !s->data_partitioning))
            s->workaround_bugs |= FF_BUG_NO_PADDING;
        else
            s->workaround_bugs &= ~FF_BUG_NO_PADDING;
    }

    if (s->msmpeg4_version || (s->workaround_bugs & FF_BUG_NO_PADDING)) { // FIXME perhaps solve this more cleanly
        int left      = get_bits_left(&s->gb);
        int max_extra = 7;

        /* no markers in M$ crap */
        if (s->msmpeg4_version && s->pict_type == AV_PICTURE_TYPE_I)
            max_extra += 17;

        /* buggy padding but the frame should still end approximately at
         * the bitstream end */
        if ((s->workaround_bugs & FF_BUG_NO_PADDING) &&
            (s->avctx->err_recognition & (AV_EF_BUFFER|AV_EF_AGGRESSIVE)))
            max_extra += 48;
        else if ((s->workaround_bugs & FF_BUG_NO_PADDING))
            max_extra += 256 * 256 * 256 * 64;

        if (left > max_extra)
            av_log(s->avctx, AV_LOG_ERROR,
                   "discarding %d junk bits at end, next would be %X\n",
                   left, show_bits(&s->gb, 24));
        else if (left < 0)
            av_log(s->avctx, AV_LOG_ERROR, "overreading %d bits\n", -left);
        else
            ff_er_add_slice(&s->er, s->resync_mb_x, s->resync_mb_y,
                            s->mb_x - 1, s->mb_y, ER_MB_END);

        return 0;
    }

    av_log(s->avctx, AV_LOG_ERROR,
           "slice end not reached but screenspace end (%d left %06X, score= %d)\n",
           get_bits_left(&s->gb), show_bits(&s->gb, 24), s->padding_bug_score);

    ff_er_add_slice(&s->er, s->resync_mb_x, s->resync_mb_y, s->mb_x, s->mb_y,
                    ER_MB_END & part_mask);

    return AVERROR_INVALIDDATA;
}
