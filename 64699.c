put_ofp14_queue_rate(struct ofpbuf *reply,
                     enum ofp14_queue_desc_prop_type type, uint16_t rate)
{
    if (rate != UINT16_MAX) {
        ofpprop_put_u16(reply, type, rate);
    }
}
