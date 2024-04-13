static int asf_read_ext_stream_properties(AVFormatContext *s, const GUIDParseTable *g)
{
    ASFContext *asf = s->priv_data;
    AVIOContext *pb = s->pb;
    AVStream *st    = NULL;
    ff_asf_guid guid;
    uint16_t nb_st_name, nb_pay_exts, st_num, lang_idx;
    int i, ret;
    uint32_t bitrate;
    uint64_t start_time, end_time, time_per_frame;
    uint64_t size = avio_rl64(pb);

    start_time = avio_rl64(pb);
    end_time   = avio_rl64(pb);
    bitrate    = avio_rl32(pb);
    avio_skip(pb, 28); // skip some unused values
    st_num     = avio_rl16(pb);
    st_num    &= ASF_STREAM_NUM;
    lang_idx   = avio_rl16(pb); // Stream Language ID Index
    for (i = 0; i < asf->nb_streams; i++) {
        if (st_num == asf->asf_st[i]->stream_index) {
            st                       = s->streams[asf->asf_st[i]->index];
            asf->asf_st[i]->lang_idx = lang_idx;
            break;
        }
    }
    time_per_frame = avio_rl64(pb); // average time per frame
    if (st) {
        st->start_time           = start_time;
        st->duration             = end_time - start_time;
        st->codecpar->bit_rate   = bitrate;
        st->avg_frame_rate.num   = 10000000;
        st->avg_frame_rate.den   = time_per_frame;
    }
    nb_st_name = avio_rl16(pb);
    nb_pay_exts   = avio_rl16(pb);
    for (i = 0; i < nb_st_name; i++) {
        uint16_t len;

        avio_rl16(pb); // Language ID Index
        len = avio_rl16(pb);
        avio_skip(pb, len);
    }

    for (i = 0; i < nb_pay_exts; i++) {
        uint32_t len;
        avio_skip(pb, 16); // Extension System ID
        avio_skip(pb, 2);  // Extension Data Size
        len = avio_rl32(pb);
        avio_skip(pb, len);
    }

    if ((ret = ff_get_guid(pb, &guid)) < 0) {
        align_position(pb, asf->offset, size);

        return 0;
    }

    g = find_guid(guid);
    if (g && !(strcmp(g->name, "Stream Properties"))) {
        if ((ret = g->read_object(s, g)) < 0)
            return ret;
    }

    align_position(pb, asf->offset, size);
    return 0;
}
