static int DecodeIPV6Packet (ThreadVars *tv, DecodeThreadVars *dtv, Packet *p, uint8_t *pkt, uint16_t len)
{
    if (unlikely(len < IPV6_HEADER_LEN)) {
        return -1;
    }

    if (unlikely(IP_GET_RAW_VER(pkt) != 6)) {
        SCLogDebug("wrong ip version %" PRIu8 "",IP_GET_RAW_VER(pkt));
        ENGINE_SET_INVALID_EVENT(p, IPV6_WRONG_IP_VER);
        return -1;
    }

    p->ip6h = (IPV6Hdr *)pkt;

    if (unlikely(len < (IPV6_HEADER_LEN + IPV6_GET_PLEN(p))))
    {
        ENGINE_SET_INVALID_EVENT(p, IPV6_TRUNC_PKT);
        return -1;
    }

    SET_IPV6_SRC_ADDR(p,&p->src);
    SET_IPV6_DST_ADDR(p,&p->dst);

    return 0;
}
