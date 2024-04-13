static int DecodeIP6inIP6(ThreadVars *tv, DecodeThreadVars *dtv, Packet *p, uint8_t *pkt, uint16_t plen, PacketQueue *pq)
{

    if (unlikely(plen < IPV6_HEADER_LEN)) {
        ENGINE_SET_INVALID_EVENT(p, IPV6_IN_IPV6_PKT_TOO_SMALL);
        return TM_ECODE_FAILED;
    }
    if (IP_GET_RAW_VER(pkt) == 6) {
        if (unlikely(pq != NULL)) {
            Packet *tp = PacketTunnelPktSetup(tv, dtv, p, pkt, plen, DECODE_TUNNEL_IPV6, pq);
            if (tp != NULL) {
                PKT_SET_SRC(tp, PKT_SRC_DECODER_IPV6);
                PacketEnqueue(pq,tp);
                StatsIncr(tv, dtv->counter_ipv6inipv6);
            }
        }
    } else {
        ENGINE_SET_EVENT(p, IPV6_IN_IPV6_WRONG_IP_VER);
    }
    return TM_ECODE_OK;
}
