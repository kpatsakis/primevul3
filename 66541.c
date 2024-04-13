void ff_mpv_reconstruct_mb(MpegEncContext *s, int16_t block[12][64])
{
#if !CONFIG_SMALL
    if(s->out_format == FMT_MPEG1) {
        if(s->avctx->lowres) mpv_reconstruct_mb_internal(s, block, 1, 1);
        else                 mpv_reconstruct_mb_internal(s, block, 0, 1);
    } else
#endif
    if(s->avctx->lowres) mpv_reconstruct_mb_internal(s, block, 1, 0);
    else                  mpv_reconstruct_mb_internal(s, block, 0, 0);
}
