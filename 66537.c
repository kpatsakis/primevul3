void ff_mpv_decode_init(MpegEncContext *s, AVCodecContext *avctx)
{
    s->avctx           = avctx;
    s->width           = avctx->coded_width;
    s->height          = avctx->coded_height;
    s->codec_id        = avctx->codec->id;
    s->workaround_bugs = avctx->workaround_bugs;

    /* convert fourcc to upper case */
    s->codec_tag          = avpriv_toupper4(avctx->codec_tag);
}
