ofputil_append_meter_config(struct ovs_list *replies,
                            const struct ofputil_meter_config *mc)
{
    struct ofpbuf *msg = ofpbuf_from_list(ovs_list_back(replies));
    size_t start_ofs = msg->size;
    struct ofp13_meter_config *reply;

    ofpbuf_put_uninit(msg, sizeof *reply);
    ofputil_put_bands(mc->n_bands, mc->bands, msg);

    reply = ofpbuf_at_assert(msg, start_ofs, sizeof *reply);
    reply->flags = htons(mc->flags);
    reply->meter_id = htonl(mc->meter_id);
    reply->length = htons(msg->size - start_ofs);

    ofpmp_postappend(replies, start_ofs);
}
