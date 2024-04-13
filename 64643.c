ofputil_pull_ofp15_buckets(struct ofpbuf *msg, size_t buckets_length,
                           enum ofp_version version, uint8_t group_type,
                           struct ovs_list *buckets)
{
    struct ofp15_bucket *ob;

    ovs_list_init(buckets);
    while (buckets_length > 0) {
        struct ofputil_bucket *bucket = NULL;
        struct ofpbuf ofpacts;
        enum ofperr err = OFPERR_OFPGMFC_BAD_BUCKET;
        size_t ob_len, actions_len, properties_len;
        ovs_be32 watch_port = ofputil_port_to_ofp11(OFPP_ANY);
        ovs_be32 watch_group = htonl(OFPG_ANY);
        ovs_be16 weight = htons(group_type == OFPGT11_SELECT ? 1 : 0);

        ofpbuf_init(&ofpacts, 0);

        ob = ofpbuf_try_pull(msg, sizeof *ob);
        if (!ob) {
            VLOG_WARN_RL(&bad_ofmsg_rl, "buckets end with %"PRIuSIZE
                         " leftover bytes", buckets_length);
            goto err;
        }

        ob_len = ntohs(ob->len);
        actions_len = ntohs(ob->action_array_len);

        if (ob_len < sizeof *ob) {
            VLOG_WARN_RL(&bad_ofmsg_rl, "OpenFlow message bucket length "
                         "%"PRIuSIZE" is not valid", ob_len);
            goto err;
        } else if (ob_len > buckets_length) {
            VLOG_WARN_RL(&bad_ofmsg_rl, "OpenFlow message bucket length "
                         "%"PRIuSIZE" exceeds remaining buckets data size %"
                         PRIuSIZE, ob_len, buckets_length);
            goto err;
        } else if (actions_len > ob_len - sizeof *ob) {
            VLOG_WARN_RL(&bad_ofmsg_rl, "OpenFlow message bucket actions "
                         "length %"PRIuSIZE" exceeds remaining bucket "
                         "data size %"PRIuSIZE, actions_len,
                         ob_len - sizeof *ob);
            goto err;
        }
        buckets_length -= ob_len;

        err = ofpacts_pull_openflow_actions(msg, actions_len, version,
                                            NULL, NULL, &ofpacts);
        if (err) {
            goto err;
        }

        properties_len = ob_len - sizeof *ob - actions_len;
        struct ofpbuf properties = ofpbuf_const_initializer(
            ofpbuf_pull(msg, properties_len), properties_len);
        while (properties.size > 0) {
            struct ofpbuf payload;
            uint64_t type;

            err = ofpprop_pull(&properties, &payload, &type);
            if (err) {
                goto err;
            }

            switch (type) {
            case OFPGBPT15_WEIGHT:
                err = ofpprop_parse_be16(&payload, &weight);
                break;

            case OFPGBPT15_WATCH_PORT:
                err = ofpprop_parse_be32(&payload, &watch_port);
                break;

            case OFPGBPT15_WATCH_GROUP:
                err = ofpprop_parse_be32(&payload, &watch_group);
                break;

            default:
                err = OFPPROP_UNKNOWN(false, "group bucket", type);
                break;
            }

            if (err) {
                goto err;
            }
        }

        bucket = xzalloc(sizeof *bucket);

        bucket->weight = ntohs(weight);
        err = ofputil_port_from_ofp11(watch_port, &bucket->watch_port);
        if (err) {
            err = OFPERR_OFPGMFC_BAD_WATCH;
            goto err;
        }
        bucket->watch_group = ntohl(watch_group);
        bucket->bucket_id = ntohl(ob->bucket_id);
        if (bucket->bucket_id > OFPG15_BUCKET_MAX) {
            VLOG_WARN_RL(&bad_ofmsg_rl, "bucket id (%u) is out of range",
                         bucket->bucket_id);
            err = OFPERR_OFPGMFC_BAD_BUCKET;
            goto err;
        }

        bucket->ofpacts = ofpbuf_steal_data(&ofpacts);
        bucket->ofpacts_len = ofpacts.size;
        ovs_list_push_back(buckets, &bucket->list_node);

        continue;

    err:
        free(bucket);
        ofpbuf_uninit(&ofpacts);
        ofputil_bucket_list_destroy(buckets);
        return err;
    }

    if (ofputil_bucket_check_duplicate_id(buckets)) {
        VLOG_WARN_RL(&bad_ofmsg_rl, "Duplicate bucket id");
        ofputil_bucket_list_destroy(buckets);
        return OFPERR_OFPGMFC_BAD_BUCKET;
    }

    return 0;
}
