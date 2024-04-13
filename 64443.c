ofputil_append_ofp11_group_desc_reply(const struct ofputil_group_desc *gds,
                                      const struct ovs_list *buckets,
                                      struct ovs_list *replies,
                                      enum ofp_version version)
{
    struct ofpbuf *reply = ofpbuf_from_list(ovs_list_back(replies));
    struct ofp11_group_desc_stats *ogds;
    struct ofputil_bucket *bucket;
    size_t start_ogds;

    start_ogds = reply->size;
    ofpbuf_put_zeros(reply, sizeof *ogds);
    LIST_FOR_EACH (bucket, list_node, buckets) {
        ofputil_put_ofp11_bucket(bucket, reply, version);
    }
    ogds = ofpbuf_at_assert(reply, start_ogds, sizeof *ogds);
    ogds->length = htons(reply->size - start_ogds);
    ogds->type = gds->type;
    ogds->group_id = htonl(gds->group_id);

    ofpmp_postappend(replies, start_ogds);
}
