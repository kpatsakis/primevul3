void ff_print_debug_info(MpegEncContext *s, Picture *p, AVFrame *pict)
{
    ff_print_debug_info2(s->avctx, pict, s->mbskip_table, p->mb_type,
                         p->qscale_table, p->motion_val, &s->low_delay,
                         s->mb_width, s->mb_height, s->mb_stride, s->quarter_sample);
}
