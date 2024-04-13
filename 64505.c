ofputil_decode_port_status(const struct ofp_header *oh,
                           struct ofputil_port_status *ps)
{
    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));
    ofpraw_pull_assert(&b);

    const struct ofp_port_status *ops = ofpbuf_pull(&b, sizeof *ops);
    if (ops->reason != OFPPR_ADD &&
        ops->reason != OFPPR_DELETE &&
        ops->reason != OFPPR_MODIFY) {
        return OFPERR_NXBRC_BAD_REASON;
    }
    ps->reason = ops->reason;

    int retval = ofputil_pull_phy_port(oh->version, &b, &ps->desc);
    ovs_assert(retval != EOF);
    return retval;
}
