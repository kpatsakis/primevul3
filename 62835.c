static int asf_read_subpayload(AVFormatContext *s, AVPacket *pkt, int is_header)
{
    ASFContext *asf = s->priv_data;
    AVIOContext *pb = s->pb;
    uint8_t sub_len;
    int ret, i;

    if (is_header) {
        asf->dts_delta = avio_r8(pb);
        if (asf->nb_mult_left) {
            asf->mult_sub_len = avio_rl16(pb); // total
        }
        asf->sub_header_offset = avio_tell(pb);
        asf->nb_sub = 0;
        asf->sub_left = 1;
    }
    sub_len = avio_r8(pb);
    if ((ret = av_get_packet(pb, pkt, sub_len)) < 0) // each subpayload is entire frame
        return ret;
    for (i = 0; i < asf->nb_streams; i++) {
        if (asf->stream_index == asf->asf_st[i]->stream_index) {
            pkt->stream_index  = asf->asf_st[i]->index;
            break;
        }
    }
    asf->return_subpayload = 1;
    if (!sub_len)
        asf->return_subpayload = 0;

    if (sub_len)
        asf->nb_sub++;
    pkt->dts = asf->sub_dts + (asf->nb_sub - 1) * asf->dts_delta - asf->preroll;
    if (asf->nb_mult_left && (avio_tell(pb) >=
                              (asf->sub_header_offset + asf->mult_sub_len))) {
        asf->sub_left = 0;
        asf->nb_mult_left--;
    }
    if (avio_tell(pb) >= asf->packet_offset + asf->packet_size - asf->pad_len) {
        asf->sub_left = 0;
        if (!asf->nb_mult_left) {
            avio_skip(pb, asf->pad_len);
            if (avio_tell(pb) != asf->packet_offset + asf->packet_size) {
                if (!asf->packet_size)
                    return AVERROR_INVALIDDATA;
                av_log(s, AV_LOG_WARNING,
                       "Position %"PRId64" wrong, should be %"PRId64"\n",
                       avio_tell(pb), asf->packet_offset + asf->packet_size);
                avio_seek(pb, asf->packet_offset + asf->packet_size, SEEK_SET);
            }
        }
    }

    return 0;
}
