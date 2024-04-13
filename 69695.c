const char *PktSrcToString(enum PktSrcEnum pkt_src)
{
    const char *pkt_src_str = "<unknown>";
    switch (pkt_src) {
        case PKT_SRC_WIRE:
            pkt_src_str = "wire/pcap";
            break;
        case PKT_SRC_DECODER_GRE:
            pkt_src_str = "gre tunnel";
            break;
        case PKT_SRC_DECODER_IPV4:
            pkt_src_str = "ipv4 tunnel";
            break;
        case PKT_SRC_DECODER_IPV6:
            pkt_src_str = "ipv6 tunnel";
            break;
        case PKT_SRC_DECODER_TEREDO:
            pkt_src_str = "teredo tunnel";
            break;
        case PKT_SRC_DEFRAG:
            pkt_src_str = "defrag";
            break;
        case PKT_SRC_STREAM_TCP_STREAM_END_PSEUDO:
            pkt_src_str = "stream";
            break;
        case PKT_SRC_STREAM_TCP_DETECTLOG_FLUSH:
            pkt_src_str = "stream (detect/log)";
            break;
        case PKT_SRC_FFR:
            pkt_src_str = "stream (flow timeout)";
            break;
    }
    return pkt_src_str;
}
