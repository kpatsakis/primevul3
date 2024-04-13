static void reset_packet(ASFPacket *asf_pkt)
{
    asf_pkt->size_left = 0;
    asf_pkt->data_size = 0;
    asf_pkt->duration  = 0;
    asf_pkt->flags     = 0;
    asf_pkt->dts       = 0;
    asf_pkt->duration  = 0;
    av_packet_unref(&asf_pkt->avpkt);
    av_init_packet(&asf_pkt->avpkt);
}
