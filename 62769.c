static av_cold int init_studio_vlcs(Mpeg4DecContext *ctx)
{
    int i, ret;

    for (i = 0; i < 12; i++) {
        ret = init_vlc(&ctx->studio_intra_tab[i], STUDIO_INTRA_BITS, 22,
                       &ff_mpeg4_studio_intra[i][0][1], 4, 2,
                       &ff_mpeg4_studio_intra[i][0][0], 4, 2,
                       0);

        if (ret < 0)
            return ret;
    }

    ret = init_vlc(&ctx->studio_luma_dc, STUDIO_INTRA_BITS, 19,
                   &ff_mpeg4_studio_dc_luma[0][1], 4, 2,
                   &ff_mpeg4_studio_dc_luma[0][0], 4, 2,
                   0);
    if (ret < 0)
        return ret;

    ret = init_vlc(&ctx->studio_chroma_dc, STUDIO_INTRA_BITS, 19,
                   &ff_mpeg4_studio_dc_chroma[0][1], 4, 2,
                   &ff_mpeg4_studio_dc_chroma[0][0], 4, 2,
                   0);
    if (ret < 0)
        return ret;

    return 0;
}
