av_cold int ff_h263_decode_init(AVCodecContext *avctx)
{
    MpegEncContext *s = avctx->priv_data;
    int ret;

    s->out_format      = FMT_H263;

    ff_mpv_decode_defaults(s);
    ff_mpv_decode_init(s, avctx);

    s->quant_precision = 5;
    s->decode_mb       = ff_h263_decode_mb;
    s->low_delay       = 1;
    s->unrestricted_mv = 1;

    /* select sub codec */
    switch (avctx->codec->id) {
    case AV_CODEC_ID_H263:
    case AV_CODEC_ID_H263P:
        s->unrestricted_mv = 0;
        avctx->chroma_sample_location = AVCHROMA_LOC_CENTER;
        break;
    case AV_CODEC_ID_MPEG4:
        break;
    case AV_CODEC_ID_MSMPEG4V1:
        s->h263_pred       = 1;
        s->msmpeg4_version = 1;
        break;
    case AV_CODEC_ID_MSMPEG4V2:
        s->h263_pred       = 1;
        s->msmpeg4_version = 2;
        break;
    case AV_CODEC_ID_MSMPEG4V3:
        s->h263_pred       = 1;
        s->msmpeg4_version = 3;
        break;
    case AV_CODEC_ID_WMV1:
        s->h263_pred       = 1;
        s->msmpeg4_version = 4;
        break;
    case AV_CODEC_ID_WMV2:
        s->h263_pred       = 1;
        s->msmpeg4_version = 5;
        break;
    case AV_CODEC_ID_VC1:
    case AV_CODEC_ID_WMV3:
    case AV_CODEC_ID_VC1IMAGE:
    case AV_CODEC_ID_WMV3IMAGE:
    case AV_CODEC_ID_MSS2:
        s->h263_pred       = 1;
        s->msmpeg4_version = 6;
        avctx->chroma_sample_location = AVCHROMA_LOC_LEFT;
        break;
    case AV_CODEC_ID_H263I:
        break;
    case AV_CODEC_ID_FLV1:
        s->h263_flv = 1;
        break;
    default:
        av_log(avctx, AV_LOG_ERROR, "Unsupported codec %d\n",
               avctx->codec->id);
        return AVERROR(ENOSYS);
    }
    s->codec_id    = avctx->codec->id;

    if (avctx->codec_tag == AV_RL32("L263") || avctx->codec_tag == AV_RL32("S263"))
        if (avctx->extradata_size == 56 && avctx->extradata[0] == 1)
            s->ehc_mode = 1;

    /* for H.263, we allocate the images after having read the header */
    if (avctx->codec->id != AV_CODEC_ID_H263 &&
        avctx->codec->id != AV_CODEC_ID_H263P &&
        avctx->codec->id != AV_CODEC_ID_MPEG4) {
        avctx->pix_fmt = h263_get_format(avctx);
        ff_mpv_idct_init(s);
        if ((ret = ff_mpv_common_init(s)) < 0)
            return ret;
    }

    ff_h263dsp_init(&s->h263dsp);
    ff_qpeldsp_init(&s->qdsp);
    ff_h263_decode_init_vlc();

    return 0;
}
