int DecodeIPV6(ThreadVars *tv, DecodeThreadVars *dtv, Packet *p, uint8_t *pkt, uint16_t len, PacketQueue *pq)
{
    int ret;

    StatsIncr(tv, dtv->counter_ipv6);

    /* do the actual decoding */
    ret = DecodeIPV6Packet (tv, dtv, p, pkt, len);
    if (unlikely(ret < 0)) {
        p->ip6h = NULL;
        return TM_ECODE_FAILED;
    }

#ifdef DEBUG
    if (SCLogDebugEnabled()) { /* only convert the addresses if debug is really enabled */
        /* debug print */
        char s[46], d[46];
        PrintInet(AF_INET6, (const void *)GET_IPV6_SRC_ADDR(p), s, sizeof(s));
        PrintInet(AF_INET6, (const void *)GET_IPV6_DST_ADDR(p), d, sizeof(d));
        SCLogDebug("IPV6 %s->%s - CLASS: %" PRIu32 " FLOW: %" PRIu32 " NH: %" PRIu32 " PLEN: %" PRIu32 " HLIM: %" PRIu32 "", s,d,
                IPV6_GET_CLASS(p), IPV6_GET_FLOW(p), IPV6_GET_NH(p), IPV6_GET_PLEN(p),
                IPV6_GET_HLIM(p));
    }
#endif /* DEBUG */

    /* now process the Ext headers and/or the L4 Layer */
    switch(IPV6_GET_NH(p)) {
        case IPPROTO_TCP:
            IPV6_SET_L4PROTO (p, IPPROTO_TCP);
            DecodeTCP(tv, dtv, p, pkt + IPV6_HEADER_LEN, IPV6_GET_PLEN(p), pq);
            return TM_ECODE_OK;
        case IPPROTO_UDP:
            IPV6_SET_L4PROTO (p, IPPROTO_UDP);
            DecodeUDP(tv, dtv, p, pkt + IPV6_HEADER_LEN, IPV6_GET_PLEN(p), pq);
            return TM_ECODE_OK;
        case IPPROTO_ICMPV6:
            IPV6_SET_L4PROTO (p, IPPROTO_ICMPV6);
            DecodeICMPV6(tv, dtv, p, pkt + IPV6_HEADER_LEN, IPV6_GET_PLEN(p), pq);
            return TM_ECODE_OK;
        case IPPROTO_SCTP:
            IPV6_SET_L4PROTO (p, IPPROTO_SCTP);
            DecodeSCTP(tv, dtv, p, pkt + IPV6_HEADER_LEN, IPV6_GET_PLEN(p), pq);
            return TM_ECODE_OK;
        case IPPROTO_IPIP:
            IPV6_SET_L4PROTO(p, IPPROTO_IPIP);
            DecodeIPv4inIPv6(tv, dtv, p, pkt + IPV6_HEADER_LEN, IPV6_GET_PLEN(p), pq);
            return TM_ECODE_OK;
        case IPPROTO_IPV6:
            DecodeIP6inIP6(tv, dtv, p, pkt + IPV6_HEADER_LEN, IPV6_GET_PLEN(p), pq);
            return TM_ECODE_OK;
        case IPPROTO_FRAGMENT:
        case IPPROTO_HOPOPTS:
        case IPPROTO_ROUTING:
        case IPPROTO_NONE:
        case IPPROTO_DSTOPTS:
        case IPPROTO_AH:
        case IPPROTO_ESP:
        case IPPROTO_MH:
        case IPPROTO_HIP:
        case IPPROTO_SHIM6:
            DecodeIPV6ExtHdrs(tv, dtv, p, pkt + IPV6_HEADER_LEN, IPV6_GET_PLEN(p), pq);
            break;
        case IPPROTO_ICMP:
            ENGINE_SET_EVENT(p,IPV6_WITH_ICMPV4);
            break;
        default:
            ENGINE_SET_EVENT(p, IPV6_UNKNOWN_NEXT_HEADER);
            IPV6_SET_L4PROTO (p, IPV6_GET_NH(p));
            break;
    }
    p->proto = IPV6_GET_L4PROTO (p);

    /* Pass to defragger if a fragment. */
    if (IPV6_EXTHDR_ISSET_FH(p)) {
        Packet *rp = Defrag(tv, dtv, p, pq);
        if (rp != NULL) {
            PacketEnqueue(pq,rp);
        }
    }

    return TM_ECODE_OK;
}
