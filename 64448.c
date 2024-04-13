ofputil_append_queue_get_config_reply(const struct ofputil_queue_config *qc,
                                      struct ovs_list *replies)
{
    enum ofp_version ofp_version = ofpmp_version(replies);
    struct ofpbuf *reply = ofpbuf_from_list(ovs_list_back(replies));
    size_t start_ofs = reply->size;
    size_t len_ofs;
    ovs_be16 *len;

    if (ofp_version < OFP14_VERSION) {
        if (ofp_version < OFP12_VERSION) {
            struct ofp10_packet_queue *opq10;

            opq10 = ofpbuf_put_zeros(reply, sizeof *opq10);
            opq10->queue_id = htonl(qc->queue);
            len_ofs = (char *) &opq10->len - (char *) reply->data;
        } else {
            struct ofp12_packet_queue *opq12;

            opq12 = ofpbuf_put_zeros(reply, sizeof *opq12);
            opq12->port = ofputil_port_to_ofp11(qc->port);
            opq12->queue_id = htonl(qc->queue);
            len_ofs = (char *) &opq12->len - (char *) reply->data;
        }

        put_ofp10_queue_rate(reply, OFPQT10_MIN_RATE, qc->min_rate);
        put_ofp10_queue_rate(reply, OFPQT11_MAX_RATE, qc->max_rate);
    } else {
        struct ofp14_queue_desc *oqd = ofpbuf_put_zeros(reply, sizeof *oqd);
        oqd->port_no = ofputil_port_to_ofp11(qc->port);
        oqd->queue_id = htonl(qc->queue);
        len_ofs = (char *) &oqd->len - (char *) reply->data;
        put_ofp14_queue_rate(reply, OFPQDPT14_MIN_RATE, qc->min_rate);
        put_ofp14_queue_rate(reply, OFPQDPT14_MAX_RATE, qc->max_rate);
    }

    len = ofpbuf_at(reply, len_ofs, sizeof *len);
    *len = htons(reply->size - start_ofs);

    if (ofp_version >= OFP14_VERSION) {
        ofpmp_postappend(replies, start_ofs);
    }
}
