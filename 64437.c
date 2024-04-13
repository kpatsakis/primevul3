ofputil_append_flow_update(const struct ofputil_flow_update *update,
                           struct ovs_list *replies,
                           const struct tun_table *tun_table)
{
    struct ofputil_flow_update *update_ =
        CONST_CAST(struct ofputil_flow_update *, update);
    const struct tun_table *orig_tun_table;
    enum ofp_version version = ofpmp_version(replies);
    struct nx_flow_update_header *nfuh;
    struct ofpbuf *msg;
    size_t start_ofs;

    orig_tun_table = update->match.flow.tunnel.metadata.tab;
    update_->match.flow.tunnel.metadata.tab = tun_table;

    msg = ofpbuf_from_list(ovs_list_back(replies));
    start_ofs = msg->size;

    if (update->event == NXFME_ABBREV) {
        struct nx_flow_update_abbrev *nfua;

        nfua = ofpbuf_put_zeros(msg, sizeof *nfua);
        nfua->xid = update->xid;
    } else {
        struct nx_flow_update_full *nfuf;
        int match_len;

        ofpbuf_put_zeros(msg, sizeof *nfuf);
        match_len = nx_put_match(msg, &update->match, htonll(0), htonll(0));
        ofpacts_put_openflow_actions(update->ofpacts, update->ofpacts_len, msg,
                                     version);
        nfuf = ofpbuf_at_assert(msg, start_ofs, sizeof *nfuf);
        nfuf->reason = htons(update->reason);
        nfuf->priority = htons(update->priority);
        nfuf->idle_timeout = htons(update->idle_timeout);
        nfuf->hard_timeout = htons(update->hard_timeout);
        nfuf->match_len = htons(match_len);
        nfuf->table_id = update->table_id;
        nfuf->cookie = update->cookie;
    }

    nfuh = ofpbuf_at_assert(msg, start_ofs, sizeof *nfuh);
    nfuh->length = htons(msg->size - start_ofs);
    nfuh->event = htons(update->event);

    ofpmp_postappend(replies, start_ofs);
    update_->match.flow.tunnel.metadata.tab = orig_tun_table;
}
