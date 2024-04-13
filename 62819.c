static int asf_read_data(AVFormatContext *s, const GUIDParseTable *g)
{
    ASFContext *asf = s->priv_data;
    AVIOContext *pb = s->pb;
    uint64_t size   = asf->data_size = avio_rl64(pb);
    int i;

    if (!asf->data_reached) {
        asf->data_reached       = 1;
        asf->data_offset        = asf->offset;
    }

    for (i = 0; i < asf->nb_streams; i++) {
        if (!(asf->b_flags & ASF_FLAG_BROADCAST))
            s->streams[i]->duration = asf->duration;
    }
    asf->nb_mult_left           = 0;
    asf->sub_left               = 0;
    asf->state                  = PARSE_PACKET_HEADER;
    asf->return_subpayload      = 0;
    asf->packet_size_internal   = 0;
    avio_skip(pb, 16); // skip File ID
    size = avio_rl64(pb); // Total Data Packets
    if (size != asf->nb_packets)
        av_log(s, AV_LOG_WARNING,
               "Number of Packets from File Properties Object is not equal to Total"
               "Datapackets value! num of packets %"PRIu64" total num %"PRIu64".\n",
               size, asf->nb_packets);
    avio_skip(pb, 2); // skip reserved field
    asf->first_packet_offset = avio_tell(pb);
    if ((pb->seekable & AVIO_SEEKABLE_NORMAL) && !(asf->b_flags & ASF_FLAG_BROADCAST))
        align_position(pb, asf->offset, asf->data_size);

    return 0;
}
