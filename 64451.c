ofputil_append_table_features_reply(const struct ofputil_table_features *tf,
                                    struct ovs_list *replies)
{
    struct ofpbuf *reply = ofpbuf_from_list(ovs_list_back(replies));
    enum ofp_version version = ofpmp_version(replies);
    size_t start_ofs = reply->size;
    struct ofp13_table_features *otf;

    otf = ofpbuf_put_zeros(reply, sizeof *otf);
    otf->table_id = tf->table_id;
    ovs_strlcpy(otf->name, tf->name, sizeof otf->name);
    otf->metadata_match = tf->metadata_match;
    otf->metadata_write = tf->metadata_write;
    if (version >= OFP14_VERSION) {
        if (tf->supports_eviction) {
            otf->capabilities |= htonl(OFPTC14_EVICTION);
        }
        if (tf->supports_vacancy_events) {
            otf->capabilities |= htonl(OFPTC14_VACANCY_EVENTS);
        }
    }
    otf->max_entries = htonl(tf->max_entries);

    put_table_instruction_features(reply, &tf->nonmiss, 0, version);
    put_table_instruction_features(reply, &tf->miss, 1, version);

    put_fields_property(reply, &tf->match, &tf->mask,
                        OFPTFPT13_MATCH, version);
    put_fields_property(reply, &tf->wildcard, NULL,
                        OFPTFPT13_WILDCARDS, version);

    otf = ofpbuf_at_assert(reply, start_ofs, sizeof *otf);
    otf->length = htons(reply->size - start_ofs);
    ofpmp_postappend(replies, start_ofs);
}
