static int init_duplicate_context(MpegEncContext *s)
{
    int y_size = s->b8_stride * (2 * s->mb_height + 1);
    int c_size = s->mb_stride * (s->mb_height + 1);
    int yc_size = y_size + 2 * c_size;
    int i;

    if (s->mb_height & 1)
        yc_size += 2*s->b8_stride + 2*s->mb_stride;

    s->sc.edge_emu_buffer =
    s->me.scratchpad   =
    s->me.temp         =
    s->sc.rd_scratchpad   =
    s->sc.b_scratchpad    =
    s->sc.obmc_scratchpad = NULL;

    if (s->encoding) {
        FF_ALLOCZ_OR_GOTO(s->avctx, s->me.map,
                          ME_MAP_SIZE * sizeof(uint32_t), fail)
        FF_ALLOCZ_OR_GOTO(s->avctx, s->me.score_map,
                          ME_MAP_SIZE * sizeof(uint32_t), fail)
        if (s->noise_reduction) {
            FF_ALLOCZ_OR_GOTO(s->avctx, s->dct_error_sum,
                              2 * 64 * sizeof(int), fail)
        }
    }
    FF_ALLOCZ_OR_GOTO(s->avctx, s->blocks, 64 * 12 * 2 * sizeof(int16_t), fail)
    s->block = s->blocks[0];

    for (i = 0; i < 12; i++) {
        s->pblocks[i] = &s->block[i];
    }

    FF_ALLOCZ_OR_GOTO(s->avctx, s->block32, sizeof(*s->block32), fail)

    if (s->avctx->codec_tag == AV_RL32("VCR2")) {
        FFSWAP(void *, s->pblocks[4], s->pblocks[5]);
    }

    if (s->out_format == FMT_H263) {
        /* ac values */
        FF_ALLOCZ_OR_GOTO(s->avctx, s->ac_val_base,
                          yc_size * sizeof(int16_t) * 16, fail);
        s->ac_val[0] = s->ac_val_base + s->b8_stride + 1;
        s->ac_val[1] = s->ac_val_base + y_size + s->mb_stride + 1;
        s->ac_val[2] = s->ac_val[1] + c_size;
    }

    return 0;
fail:
    return -1; // free() through ff_mpv_common_end()
}
