parse_ofp10_queue_rate(const struct ofp10_queue_prop_header *hdr,
                       uint16_t *rate)
{
    const struct ofp10_queue_prop_rate *oqpr;

    if (hdr->len == htons(sizeof *oqpr)) {
        oqpr = (const struct ofp10_queue_prop_rate *) hdr;
        *rate = ntohs(oqpr->rate);
        return 0;
    } else {
        return OFPERR_OFPBRC_BAD_LEN;
    }
}
