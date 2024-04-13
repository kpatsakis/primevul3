int ff_update_duplicate_context(MpegEncContext *dst, MpegEncContext *src)
{
    MpegEncContext bak;
    int i, ret;
    backup_duplicate_context(&bak, dst);
    memcpy(dst, src, sizeof(MpegEncContext));
    backup_duplicate_context(dst, &bak);
    for (i = 0; i < 12; i++) {
        dst->pblocks[i] = &dst->block[i];
    }
    if (dst->avctx->codec_tag == AV_RL32("VCR2")) {
        FFSWAP(void *, dst->pblocks[4], dst->pblocks[5]);
    }
    if (!dst->sc.edge_emu_buffer &&
        (ret = ff_mpeg_framesize_alloc(dst->avctx, &dst->me,
                                       &dst->sc, dst->linesize)) < 0) {
        av_log(dst->avctx, AV_LOG_ERROR, "failed to allocate context "
               "scratch buffers.\n");
        return ret;
    }
    return 0;
}
