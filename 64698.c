put_ofp10_queue_rate(struct ofpbuf *reply,
                     enum ofp10_queue_properties property, uint16_t rate)
{
    if (rate != UINT16_MAX) {
        struct ofp10_queue_prop_rate *oqpr;

        oqpr = ofpbuf_put_zeros(reply, sizeof *oqpr);
        oqpr->prop_header.property = htons(property);
        oqpr->prop_header.len = htons(sizeof *oqpr);
        oqpr->rate = htons(rate);
    }
}
