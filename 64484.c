ofputil_decode_meter_config(struct ofpbuf *msg,
                            struct ofputil_meter_config *mc,
                            struct ofpbuf *bands)
{
    const struct ofp13_meter_config *omc;
    enum ofperr err;

    /* Pull OpenFlow headers for the first call. */
    if (!msg->header) {
        ofpraw_pull_assert(msg);
    }

    if (!msg->size) {
        return EOF;
    }

    omc = ofpbuf_try_pull(msg, sizeof *omc);
    if (!omc) {
        VLOG_WARN_RL(&bad_ofmsg_rl,
                     "OFPMP_METER_CONFIG reply has %"PRIu32" leftover bytes at end",
                     msg->size);
        return OFPERR_OFPBRC_BAD_LEN;
    }

    ofpbuf_clear(bands);
    err = ofputil_pull_bands(msg, ntohs(omc->length) - sizeof *omc,
                             &mc->n_bands, bands);
    if (err) {
        return err;
    }
    mc->meter_id = ntohl(omc->meter_id);
    mc->flags = ntohs(omc->flags);
    mc->bands = bands->data;

    return 0;
}
