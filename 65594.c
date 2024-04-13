void ff_er_frame_start(ERContext *s)
{
    if (!s->avctx->error_concealment)
        return;

    if (!s->mecc_inited) {
        ff_me_cmp_init(&s->mecc, s->avctx);
        s->mecc_inited = 1;
    }

    memset(s->error_status_table, ER_MB_ERROR | VP_START | ER_MB_END,
           s->mb_stride * s->mb_height * sizeof(uint8_t));
    atomic_init(&s->error_count, 3 * s->mb_num);
    s->error_occurred = 0;
}
