ofputil_pull_ofp14_queue_stats(struct ofputil_queue_stats *oqs,
                               struct ofpbuf *msg)
{
    const struct ofp14_queue_stats *qs14;
    size_t len;

    qs14 = ofpbuf_try_pull(msg, sizeof *qs14);
    if (!qs14) {
        return OFPERR_OFPBRC_BAD_LEN;
    }

    len = ntohs(qs14->length);
    if (len < sizeof *qs14 || len - sizeof *qs14 > msg->size) {
        return OFPERR_OFPBRC_BAD_LEN;
    }
    ofpbuf_pull(msg, len - sizeof *qs14);

    /* No properties yet defined, so ignore them for now. */

    return ofputil_queue_stats_from_ofp13(oqs, &qs14->qs);
}
