static int pva_read_header(AVFormatContext *s) {
    AVStream *st;

    if (!(st = avformat_new_stream(s, NULL)))
        return AVERROR(ENOMEM);
    st->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    st->codecpar->codec_id   = AV_CODEC_ID_MPEG2VIDEO;
    st->need_parsing      = AVSTREAM_PARSE_FULL;
    avpriv_set_pts_info(st, 32, 1, 90000);
    av_add_index_entry(st, 0, 0, 0, 0, AVINDEX_KEYFRAME);

    if (!(st = avformat_new_stream(s, NULL)))
        return AVERROR(ENOMEM);
    st->codecpar->codec_type = AVMEDIA_TYPE_AUDIO;
    st->codecpar->codec_id   = AV_CODEC_ID_MP2;
    st->need_parsing      = AVSTREAM_PARSE_FULL;
    avpriv_set_pts_info(st, 33, 1, 90000);
    av_add_index_entry(st, 0, 0, 0, 0, AVINDEX_KEYFRAME);

    /* the parameters will be extracted from the compressed bitstream */
    return 0;
}
