void DecodeIPV6FragHeader(Packet *p, uint8_t *pkt,
                          uint16_t hdrextlen, uint16_t plen,
                          uint16_t prev_hdrextlen)
{
    uint16_t frag_offset = (*(pkt + 2) << 8 | *(pkt + 3)) & 0xFFF8;
    int frag_morefrags   = (*(pkt + 2) << 8 | *(pkt + 3)) & 0x0001;

    p->ip6eh.fh_offset = frag_offset;
    p->ip6eh.fh_more_frags_set = frag_morefrags ? TRUE : FALSE;
    p->ip6eh.fh_nh = *pkt;

    uint32_t fh_id;
    memcpy(&fh_id, pkt+4, 4);
    p->ip6eh.fh_id = SCNtohl(fh_id);

    SCLogDebug("IPV6 FH: offset %u, mf %s, nh %u, id %u/%x",
            p->ip6eh.fh_offset,
            p->ip6eh.fh_more_frags_set ? "true" : "false",
            p->ip6eh.fh_nh,
            p->ip6eh.fh_id, p->ip6eh.fh_id);

    uint16_t frag_hdr_offset = (uint16_t)(pkt - GET_PKT_DATA(p));
    uint16_t data_offset = (uint16_t)(frag_hdr_offset + hdrextlen);
    uint16_t data_len = plen - hdrextlen;

    p->ip6eh.fh_header_offset = frag_hdr_offset;
    p->ip6eh.fh_data_offset = data_offset;
    p->ip6eh.fh_data_len = data_len;

    /* if we have a prev hdr, store the type and offset of it */
    if (prev_hdrextlen) {
        p->ip6eh.fh_prev_hdr_offset = frag_hdr_offset - prev_hdrextlen;
    }

    SCLogDebug("IPV6 FH: frag_hdr_offset %u, data_offset %u, data_len %u",
            p->ip6eh.fh_header_offset, p->ip6eh.fh_data_offset,
            p->ip6eh.fh_data_len);
}
