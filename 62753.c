static av_cold int decode_end(AVCodecContext *avctx)
{
    Mpeg4DecContext *ctx = avctx->priv_data;
    int i;

    if (!avctx->internal->is_copy) {
        for (i = 0; i < 12; i++)
            ff_free_vlc(&ctx->studio_intra_tab[i]);

        ff_free_vlc(&ctx->studio_luma_dc);
        ff_free_vlc(&ctx->studio_chroma_dc);
    }

    return ff_h263_decode_end(avctx);
}
