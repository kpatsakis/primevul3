static int detect_unknown_subobject(AVFormatContext *s, int64_t offset, int64_t size)
{
    ASFContext *asf = s->priv_data;
    AVIOContext *pb = s->pb;
    const GUIDParseTable *g = NULL;
    ff_asf_guid guid;
    int ret;

    while (avio_tell(pb) <= offset + size) {
        if (avio_tell(pb) == asf->offset)
            break;
        asf->offset = avio_tell(pb);
        if ((ret = ff_get_guid(pb, &guid)) < 0)
            return ret;
        g = find_guid(guid);
        if (g) {
            if ((ret = g->read_object(s, g)) < 0)
                return ret;
        } else {
            GUIDParseTable g2;

            g2.name         = "Unknown";
            g2.is_subobject = 1;
            asf_read_unknown(s, &g2);
        }
    }

    return 0;
}
