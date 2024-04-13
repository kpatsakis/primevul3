ofputil_append_port_desc_stats_reply(const struct ofputil_phy_port *pp,
                                     struct ovs_list *replies)
{
    struct ofpbuf *reply = ofpbuf_from_list(ovs_list_back(replies));
    size_t start_ofs = reply->size;

    ofputil_put_phy_port(ofpmp_version(replies), pp, reply);
    ofpmp_postappend(replies, start_ofs);
}
