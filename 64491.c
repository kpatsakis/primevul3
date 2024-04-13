ofputil_decode_ofp11_group_desc_reply(struct ofputil_group_desc *gd,
                                      struct ofpbuf *msg,
                                      enum ofp_version version)
{
    struct ofp11_group_desc_stats *ogds;
    size_t length;

    if (!msg->header) {
        ofpraw_pull_assert(msg);
    }

    if (!msg->size) {
        return EOF;
    }

    ogds = ofpbuf_try_pull(msg, sizeof *ogds);
    if (!ogds) {
        VLOG_WARN_RL(&bad_ofmsg_rl, "OFPST11_GROUP_DESC reply has %"PRIu32" "
                     "leftover bytes at end", msg->size);
        return OFPERR_OFPBRC_BAD_LEN;
    }
    gd->type = ogds->type;
    gd->group_id = ntohl(ogds->group_id);

    length = ntohs(ogds->length);
    if (length < sizeof *ogds || length - sizeof *ogds > msg->size) {
        VLOG_WARN_RL(&bad_ofmsg_rl, "OFPST11_GROUP_DESC reply claims invalid "
                     "length %"PRIuSIZE, length);
        return OFPERR_OFPBRC_BAD_LEN;
    }

    return ofputil_pull_ofp11_buckets(msg, length - sizeof *ogds, version,
                                      &gd->buckets);
}
