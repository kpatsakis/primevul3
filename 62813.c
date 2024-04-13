static int pva_read_packet(AVFormatContext *s, AVPacket *pkt) {
    AVIOContext *pb = s->pb;
    int64_t pva_pts;
    int ret, length, streamid;

    if (read_part_of_packet(s, &pva_pts, &length, &streamid, 1) < 0 ||
       (ret = av_get_packet(pb, pkt, length)) <= 0)
        return AVERROR(EIO);

    pkt->stream_index = streamid - 1;
    pkt->pts = pva_pts;

    return ret;
}
