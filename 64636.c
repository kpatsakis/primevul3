ofputil_pull_bands(struct ofpbuf *msg, size_t len, uint16_t *n_bands,
                   struct ofpbuf *bands)
{
    const struct ofp13_meter_band_header *ombh;
    struct ofputil_meter_band *mb;
    uint16_t n = 0;

    ombh = ofpbuf_try_pull(msg, len);
    if (!ombh) {
        return OFPERR_OFPBRC_BAD_LEN;
    }

    while (len >= sizeof (struct ofp13_meter_band_drop)) {
        size_t ombh_len = ntohs(ombh->len);
        /* All supported band types have the same length. */
        if (ombh_len != sizeof (struct ofp13_meter_band_drop)) {
            return OFPERR_OFPBRC_BAD_LEN;
        }
        mb = ofpbuf_put_uninit(bands, sizeof *mb);
        mb->type = ntohs(ombh->type);
        if (mb->type != OFPMBT13_DROP && mb->type != OFPMBT13_DSCP_REMARK) {
            return OFPERR_OFPMMFC_BAD_BAND;
        }
        mb->rate = ntohl(ombh->rate);
        mb->burst_size = ntohl(ombh->burst_size);
        mb->prec_level = (mb->type == OFPMBT13_DSCP_REMARK) ?
            ((struct ofp13_meter_band_dscp_remark *)ombh)->prec_level : 0;
        n++;
        len -= ombh_len;
        ombh = ALIGNED_CAST(struct ofp13_meter_band_header *,
                            (char *) ombh + ombh_len);
    }
    if (len) {
        return OFPERR_OFPBRC_BAD_LEN;
    }
    *n_bands = n;
    return 0;
}
