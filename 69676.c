static void DecodeIPv4inIPv6(ThreadVars *tv, DecodeThreadVars *dtv, Packet *p, uint8_t *pkt, uint16_t plen, PacketQueue *pq)
{

    if (unlikely(plen < IPV4_HEADER_LEN)) {
        ENGINE_SET_INVALID_EVENT(p, IPV4_IN_IPV6_PKT_TOO_SMALL);
        return;
    }
    if (IP_GET_RAW_VER(pkt) == 4) {
        if (pq != NULL) {
            Packet *tp = PacketTunnelPktSetup(tv, dtv, p, pkt, plen, DECODE_TUNNEL_IPV4, pq);
            if (tp != NULL) {
                PKT_SET_SRC(tp, PKT_SRC_DECODER_IPV6);
                /* add the tp to the packet queue. */
                PacketEnqueue(pq,tp);
                StatsIncr(tv, dtv->counter_ipv4inipv6);
                return;
            }
        }
    } else {
        ENGINE_SET_EVENT(p, IPV4_IN_IPV6_WRONG_IP_VER);
    }
    return;
}
