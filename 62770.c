static inline int mpeg4_decode_block(Mpeg4DecContext *ctx, int16_t *block,
                                     int n, int coded, int intra, int rvlc)
{
    MpegEncContext *s = &ctx->m;
    int level, i, last, run, qmul, qadd;
    int av_uninit(dc_pred_dir);
    RLTable *rl;
    RL_VLC_ELEM *rl_vlc;
    const uint8_t *scan_table;


    if (intra) {
        if (ctx->use_intra_dc_vlc) {
            /* DC coef */
            if (s->partitioned_frame) {
                level = s->dc_val[0][s->block_index[n]];
                if (n < 4)
                    level = FASTDIV((level + (s->y_dc_scale >> 1)), s->y_dc_scale);
                else
                    level = FASTDIV((level + (s->c_dc_scale >> 1)), s->c_dc_scale);
                dc_pred_dir = (s->pred_dir_table[s->mb_x + s->mb_y * s->mb_stride] << n) & 32;
            } else {
                level = mpeg4_decode_dc(s, n, &dc_pred_dir);
                if (level < 0)
                    return level;
            }
            block[0] = level;
            i        = 0;
        } else {
            i = -1;
            ff_mpeg4_pred_dc(s, n, 0, &dc_pred_dir, 0);
        }
        if (!coded)
            goto not_coded;

        if (rvlc) {
            rl     = &ff_rvlc_rl_intra;
            rl_vlc = ff_rvlc_rl_intra.rl_vlc[0];
        } else {
            rl     = &ff_mpeg4_rl_intra;
            rl_vlc = ff_mpeg4_rl_intra.rl_vlc[0];
        }
        if (s->ac_pred) {
            if (dc_pred_dir == 0)
                scan_table = s->intra_v_scantable.permutated;  /* left */
            else
                scan_table = s->intra_h_scantable.permutated;  /* top */
        } else {
            scan_table = s->intra_scantable.permutated;
        }
        qmul = 1;
        qadd = 0;
    } else {
        i = -1;
        if (!coded) {
            s->block_last_index[n] = i;
            return 0;
        }
        if (rvlc)
            rl = &ff_rvlc_rl_inter;
        else
            rl = &ff_h263_rl_inter;

        scan_table = s->intra_scantable.permutated;

        if (s->mpeg_quant) {
            qmul = 1;
            qadd = 0;
            if (rvlc)
                rl_vlc = ff_rvlc_rl_inter.rl_vlc[0];
            else
                rl_vlc = ff_h263_rl_inter.rl_vlc[0];
        } else {
            qmul = s->qscale << 1;
            qadd = (s->qscale - 1) | 1;
            if (rvlc)
                rl_vlc = ff_rvlc_rl_inter.rl_vlc[s->qscale];
            else
                rl_vlc = ff_h263_rl_inter.rl_vlc[s->qscale];
        }
    }
    {
        OPEN_READER(re, &s->gb);
        for (;;) {
            UPDATE_CACHE(re, &s->gb);
            GET_RL_VLC(level, run, re, &s->gb, rl_vlc, TEX_VLC_BITS, 2, 0);
            if (level == 0) {
                /* escape */
                if (rvlc) {
                    if (SHOW_UBITS(re, &s->gb, 1) == 0) {
                        av_log(s->avctx, AV_LOG_ERROR,
                               "1. marker bit missing in rvlc esc\n");
                        return AVERROR_INVALIDDATA;
                    }
                    SKIP_CACHE(re, &s->gb, 1);

                    last = SHOW_UBITS(re, &s->gb, 1);
                    SKIP_CACHE(re, &s->gb, 1);
                    run = SHOW_UBITS(re, &s->gb, 6);
                    SKIP_COUNTER(re, &s->gb, 1 + 1 + 6);
                    UPDATE_CACHE(re, &s->gb);

                    if (SHOW_UBITS(re, &s->gb, 1) == 0) {
                        av_log(s->avctx, AV_LOG_ERROR,
                               "2. marker bit missing in rvlc esc\n");
                        return AVERROR_INVALIDDATA;
                    }
                    SKIP_CACHE(re, &s->gb, 1);

                    level = SHOW_UBITS(re, &s->gb, 11);
                    SKIP_CACHE(re, &s->gb, 11);

                    if (SHOW_UBITS(re, &s->gb, 5) != 0x10) {
                        av_log(s->avctx, AV_LOG_ERROR, "reverse esc missing\n");
                        return AVERROR_INVALIDDATA;
                    }
                    SKIP_CACHE(re, &s->gb, 5);

                    level = level * qmul + qadd;
                    level = (level ^ SHOW_SBITS(re, &s->gb, 1)) - SHOW_SBITS(re, &s->gb, 1);
                    SKIP_COUNTER(re, &s->gb, 1 + 11 + 5 + 1);

                    i += run + 1;
                    if (last)
                        i += 192;
                } else {
                    int cache;
                    cache = GET_CACHE(re, &s->gb);

                    if (IS_3IV1)
                        cache ^= 0xC0000000;

                    if (cache & 0x80000000) {
                        if (cache & 0x40000000) {
                            /* third escape */
                            SKIP_CACHE(re, &s->gb, 2);
                            last = SHOW_UBITS(re, &s->gb, 1);
                            SKIP_CACHE(re, &s->gb, 1);
                            run = SHOW_UBITS(re, &s->gb, 6);
                            SKIP_COUNTER(re, &s->gb, 2 + 1 + 6);
                            UPDATE_CACHE(re, &s->gb);

                            if (IS_3IV1) {
                                level = SHOW_SBITS(re, &s->gb, 12);
                                LAST_SKIP_BITS(re, &s->gb, 12);
                            } else {
                                if (SHOW_UBITS(re, &s->gb, 1) == 0) {
                                    av_log(s->avctx, AV_LOG_ERROR,
                                           "1. marker bit missing in 3. esc\n");
                                    if (!(s->avctx->err_recognition & AV_EF_IGNORE_ERR))
                                        return AVERROR_INVALIDDATA;
                                }
                                SKIP_CACHE(re, &s->gb, 1);

                                level = SHOW_SBITS(re, &s->gb, 12);
                                SKIP_CACHE(re, &s->gb, 12);

                                if (SHOW_UBITS(re, &s->gb, 1) == 0) {
                                    av_log(s->avctx, AV_LOG_ERROR,
                                           "2. marker bit missing in 3. esc\n");
                                    if (!(s->avctx->err_recognition & AV_EF_IGNORE_ERR))
                                        return AVERROR_INVALIDDATA;
                                }

                                SKIP_COUNTER(re, &s->gb, 1 + 12 + 1);
                            }

#if 0
                            if (s->error_recognition >= FF_ER_COMPLIANT) {
                                const int abs_level= FFABS(level);
                                if (abs_level<=MAX_LEVEL && run<=MAX_RUN) {
                                    const int run1= run - rl->max_run[last][abs_level] - 1;
                                    if (abs_level <= rl->max_level[last][run]) {
                                        av_log(s->avctx, AV_LOG_ERROR, "illegal 3. esc, vlc encoding possible\n");
                                        return AVERROR_INVALIDDATA;
                                    }
                                    if (s->error_recognition > FF_ER_COMPLIANT) {
                                        if (abs_level <= rl->max_level[last][run]*2) {
                                            av_log(s->avctx, AV_LOG_ERROR, "illegal 3. esc, esc 1 encoding possible\n");
                                            return AVERROR_INVALIDDATA;
                                        }
                                        if (run1 >= 0 && abs_level <= rl->max_level[last][run1]) {
                                            av_log(s->avctx, AV_LOG_ERROR, "illegal 3. esc, esc 2 encoding possible\n");
                                            return AVERROR_INVALIDDATA;
                                        }
                                    }
                                }
                            }
#endif
                            if (level > 0)
                                level = level * qmul + qadd;
                            else
                                level = level * qmul - qadd;

                            if ((unsigned)(level + 2048) > 4095) {
                                if (s->avctx->err_recognition & (AV_EF_BITSTREAM|AV_EF_AGGRESSIVE)) {
                                    if (level > 2560 || level < -2560) {
                                        av_log(s->avctx, AV_LOG_ERROR,
                                               "|level| overflow in 3. esc, qp=%d\n",
                                               s->qscale);
                                        return AVERROR_INVALIDDATA;
                                    }
                                }
                                level = level < 0 ? -2048 : 2047;
                            }

                            i += run + 1;
                            if (last)
                                i += 192;
                        } else {
                            /* second escape */
                            SKIP_BITS(re, &s->gb, 2);
                            GET_RL_VLC(level, run, re, &s->gb, rl_vlc, TEX_VLC_BITS, 2, 1);
                            i    += run + rl->max_run[run >> 7][level / qmul] + 1;  // FIXME opt indexing
                            level = (level ^ SHOW_SBITS(re, &s->gb, 1)) - SHOW_SBITS(re, &s->gb, 1);
                            LAST_SKIP_BITS(re, &s->gb, 1);
                        }
                    } else {
                        /* first escape */
                        SKIP_BITS(re, &s->gb, 1);
                        GET_RL_VLC(level, run, re, &s->gb, rl_vlc, TEX_VLC_BITS, 2, 1);
                        i    += run;
                        level = level + rl->max_level[run >> 7][(run - 1) & 63] * qmul;  // FIXME opt indexing
                        level = (level ^ SHOW_SBITS(re, &s->gb, 1)) - SHOW_SBITS(re, &s->gb, 1);
                        LAST_SKIP_BITS(re, &s->gb, 1);
                    }
                }
            } else {
                i    += run;
                level = (level ^ SHOW_SBITS(re, &s->gb, 1)) - SHOW_SBITS(re, &s->gb, 1);
                LAST_SKIP_BITS(re, &s->gb, 1);
            }
            ff_tlog(s->avctx, "dct[%d][%d] = %- 4d end?:%d\n", scan_table[i&63]&7, scan_table[i&63] >> 3, level, i>62);
            if (i > 62) {
                i -= 192;
                if (i & (~63)) {
                    av_log(s->avctx, AV_LOG_ERROR,
                           "ac-tex damaged at %d %d\n", s->mb_x, s->mb_y);
                    return AVERROR_INVALIDDATA;
                }

                block[scan_table[i]] = level;
                break;
            }

            block[scan_table[i]] = level;
        }
        CLOSE_READER(re, &s->gb);
    }

not_coded:
    if (intra) {
        if (!ctx->use_intra_dc_vlc) {
            block[0] = ff_mpeg4_pred_dc(s, n, block[0], &dc_pred_dir, 0);

            i -= i >> 31;  // if (i == -1) i = 0;
        }

        ff_mpeg4_pred_ac(s, block, n, dc_pred_dir);
        if (s->ac_pred)
            i = 63;  // FIXME not optimal
    }
    s->block_last_index[n] = i;
    return 0;
}
