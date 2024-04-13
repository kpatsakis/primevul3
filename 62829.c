static int asf_read_properties(AVFormatContext *s, const GUIDParseTable *g)
{
    ASFContext *asf = s->priv_data;
    AVIOContext *pb = s->pb;
    time_t creation_time;

    avio_rl64(pb); // read object size
    avio_skip(pb, 16); // skip File ID
    avio_skip(pb, 8);  // skip File size
    creation_time = avio_rl64(pb);
    if (!(asf->b_flags & ASF_FLAG_BROADCAST)) {
        struct tm tmbuf;
        struct tm *tm;
        char buf[64];

        creation_time /= 10000000;
        creation_time -= 11644473600;
        tm = gmtime_r(&creation_time, &tmbuf);
        if (tm) {
            if (!strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm))
                buf[0] = '\0';
        } else
            buf[0] = '\0';
        if (buf[0]) {
            if (av_dict_set(&s->metadata, "creation_time", buf, 0) < 0)
                av_log(s, AV_LOG_WARNING, "av_dict_set failed.\n");
        }
    }
    asf->nb_packets  = avio_rl64(pb);
    asf->duration    = avio_rl64(pb) / 10000; // stream duration
    avio_skip(pb, 8); // skip send duration
    asf->preroll     = avio_rl64(pb);
    asf->duration   -= asf->preroll;
    asf->b_flags     = avio_rl32(pb);
    avio_skip(pb, 4); // skip minimal packet size
    asf->packet_size  = avio_rl32(pb);
    avio_skip(pb, 4); // skip max_bitrate

    return 0;
}
