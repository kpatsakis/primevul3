static int asf_read_seek(AVFormatContext *s, int stream_index,
                         int64_t timestamp, int flags)
{
    ASFContext *asf = s->priv_data;
    int idx, ret;

    if (s->streams[stream_index]->nb_index_entries && asf->is_simple_index) {
        idx = av_index_search_timestamp(s->streams[stream_index], timestamp, flags);
        if (idx < 0 || idx >= s->streams[stream_index]->nb_index_entries)
            return AVERROR_INVALIDDATA;
        avio_seek(s->pb, s->streams[stream_index]->index_entries[idx].pos, SEEK_SET);
    } else {
        if ((ret = ff_seek_frame_binary(s, stream_index, timestamp, flags)) < 0)
            return ret;
    }

    reset_packet_state(s);

    return 0;
}
