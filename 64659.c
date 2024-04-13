ofputil_put_ofp15_bucket(const struct ofputil_bucket *bucket,
                         uint32_t bucket_id, enum ofp11_group_type group_type,
                         struct ofpbuf *openflow, enum ofp_version ofp_version)
{
    struct ofp15_bucket *ob;
    size_t start, actions_start, actions_len;

    start = openflow->size;
    ofpbuf_put_zeros(openflow, sizeof *ob);

    actions_start = openflow->size;
    ofpacts_put_openflow_actions(bucket->ofpacts, bucket->ofpacts_len,
                                 openflow, ofp_version);
    actions_len = openflow->size - actions_start;

    if (group_type == OFPGT11_SELECT) {
        ofpprop_put_u16(openflow, OFPGBPT15_WEIGHT, bucket->weight);
    }
    if (bucket->watch_port != OFPP_ANY) {
        ofpprop_put_be32(openflow, OFPGBPT15_WATCH_PORT,
                         ofputil_port_to_ofp11(bucket->watch_port));
    }
    if (bucket->watch_group != OFPG_ANY) {
        ofpprop_put_u32(openflow, OFPGBPT15_WATCH_GROUP, bucket->watch_group);
    }

    ob = ofpbuf_at_assert(openflow, start, sizeof *ob);
    ob->len = htons(openflow->size - start);
    ob->action_array_len = htons(actions_len);
    ob->bucket_id = htonl(bucket_id);
}
