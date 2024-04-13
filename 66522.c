static av_cold int dct_init(MpegEncContext *s)
{
    ff_blockdsp_init(&s->bdsp, s->avctx);
    ff_h264chroma_init(&s->h264chroma, 8); //for lowres
    ff_hpeldsp_init(&s->hdsp, s->avctx->flags);
    ff_mpegvideodsp_init(&s->mdsp);
    ff_videodsp_init(&s->vdsp, s->avctx->bits_per_raw_sample);

    if (s->avctx->debug & FF_DEBUG_NOMC) {
        int i;
        for (i=0; i<4; i++) {
            s->hdsp.avg_pixels_tab[0][i] = gray16;
            s->hdsp.put_pixels_tab[0][i] = gray16;
            s->hdsp.put_no_rnd_pixels_tab[0][i] = gray16;

            s->hdsp.avg_pixels_tab[1][i] = gray8;
            s->hdsp.put_pixels_tab[1][i] = gray8;
            s->hdsp.put_no_rnd_pixels_tab[1][i] = gray8;
        }
    }

    s->dct_unquantize_h263_intra = dct_unquantize_h263_intra_c;
    s->dct_unquantize_h263_inter = dct_unquantize_h263_inter_c;
    s->dct_unquantize_mpeg1_intra = dct_unquantize_mpeg1_intra_c;
    s->dct_unquantize_mpeg1_inter = dct_unquantize_mpeg1_inter_c;
    s->dct_unquantize_mpeg2_intra = dct_unquantize_mpeg2_intra_c;
    if (s->avctx->flags & AV_CODEC_FLAG_BITEXACT)
        s->dct_unquantize_mpeg2_intra = dct_unquantize_mpeg2_intra_bitexact;
    s->dct_unquantize_mpeg2_inter = dct_unquantize_mpeg2_inter_c;

    if (HAVE_INTRINSICS_NEON)
        ff_mpv_common_init_neon(s);

    if (ARCH_ALPHA)
        ff_mpv_common_init_axp(s);
    if (ARCH_ARM)
        ff_mpv_common_init_arm(s);
    if (ARCH_PPC)
        ff_mpv_common_init_ppc(s);
    if (ARCH_X86)
        ff_mpv_common_init_x86(s);
    if (ARCH_MIPS)
        ff_mpv_common_init_mips(s);

    return 0;
}
