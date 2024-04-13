static int asf_read_marker(AVFormatContext *s, const GUIDParseTable *g)
{
    ASFContext *asf = s->priv_data;
    AVIOContext *pb = s->pb;
    uint64_t size   = avio_rl64(pb);
    int i, nb_markers, ret;
    size_t len;
    char name[1024];

    avio_skip(pb, 8);
    avio_skip(pb, 8); // skip reserved GUID
    nb_markers = avio_rl32(pb);
    avio_skip(pb, 2); // skip reserved field
    len = avio_rl16(pb);
    for (i = 0; i < len; i++)
        avio_skip(pb, 1);

    for (i = 0; i < nb_markers; i++) {
        int64_t pts;

        avio_skip(pb, 8);
        pts = avio_rl64(pb);
        pts -= asf->preroll * 10000;
        avio_skip(pb, 2); // entry length
        avio_skip(pb, 4); // send time
        avio_skip(pb, 4); // flags
        len = avio_rl32(pb);

        if ((ret = avio_get_str16le(pb, len, name,
                                    sizeof(name))) < len)
            avio_skip(pb, len - ret);
        avpriv_new_chapter(s, i, (AVRational) { 1, 10000000 }, pts,
                           AV_NOPTS_VALUE, name);
    }
    align_position(pb, asf->offset, size);

    return 0;
}
