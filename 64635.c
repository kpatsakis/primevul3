ofputil_pull_band_stats(struct ofpbuf *msg, size_t len, uint16_t *n_bands,
                        struct ofpbuf *bands)
{
    const struct ofp13_meter_band_stats *ombs;
    struct ofputil_meter_band_stats *mbs;
    uint16_t n, i;

    ombs = ofpbuf_try_pull(msg, len);
    if (!ombs) {
        return OFPERR_OFPBRC_BAD_LEN;
    }

    n = len / sizeof *ombs;
    if (len != n * sizeof *ombs) {
        return OFPERR_OFPBRC_BAD_LEN;
    }

    mbs = ofpbuf_put_uninit(bands, len);

    for (i = 0; i < n; ++i) {
        mbs[i].packet_count = ntohll(ombs[i].packet_band_count);
        mbs[i].byte_count = ntohll(ombs[i].byte_band_count);
    }
    *n_bands = n;
    return 0;
}
