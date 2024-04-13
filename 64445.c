ofputil_append_ofp15_group_desc_reply(const struct ofputil_group_desc *gds,
                                      const struct ovs_list *buckets,
                                      struct ovs_list *replies,
                                      enum ofp_version version)
{
    struct ofpbuf *reply = ofpbuf_from_list(ovs_list_back(replies));
    struct ofp15_group_desc_stats *ogds;
    struct ofputil_bucket *bucket;
    size_t start_ogds, start_buckets;

    start_ogds = reply->size;
    ofpbuf_put_zeros(reply, sizeof *ogds);
    start_buckets = reply->size;
    LIST_FOR_EACH (bucket, list_node, buckets) {
        ofputil_put_ofp15_bucket(bucket, bucket->bucket_id,
                                 gds->type, reply, version);
    }
    ogds = ofpbuf_at_assert(reply, start_ogds, sizeof *ogds);
    ogds->type = gds->type;
    ogds->group_id = htonl(gds->group_id);
    ogds->bucket_list_len =  htons(reply->size - start_buckets);

    /* Add group properties */
    if (gds->props.selection_method[0]) {
        ofputil_put_group_prop_ntr_selection_method(version, &gds->props,
                                                    reply);
    }
    ogds = ofpbuf_at_assert(reply, start_ogds, sizeof *ogds);
    ogds->length = htons(reply->size - start_ogds);

    ofpmp_postappend(replies, start_ogds);
}
