ofputil_decode_table_features(struct ofpbuf *msg,
                              struct ofputil_table_features *tf, bool loose)
{
    memset(tf, 0, sizeof *tf);

    if (!msg->header) {
        ofpraw_pull_assert(msg);
    }

    if (!msg->size) {
        return EOF;
    }

    const struct ofp_header *oh = msg->header;
    struct ofp13_table_features *otf = msg->data;
    if (msg->size < sizeof *otf) {
        return OFPERR_OFPBPC_BAD_LEN;
    }

    unsigned int len = ntohs(otf->length);
    if (len < sizeof *otf || len % 8 || len > msg->size) {
        return OFPERR_OFPBPC_BAD_LEN;
    }

    tf->table_id = otf->table_id;
    if (tf->table_id == OFPTT_ALL) {
        return OFPERR_OFPTFFC_BAD_TABLE;
    }

    ovs_strlcpy(tf->name, otf->name, OFP_MAX_TABLE_NAME_LEN);
    tf->metadata_match = otf->metadata_match;
    tf->metadata_write = otf->metadata_write;
    tf->miss_config = OFPUTIL_TABLE_MISS_DEFAULT;
    if (oh->version >= OFP14_VERSION) {
        uint32_t caps = ntohl(otf->capabilities);
        tf->supports_eviction = (caps & OFPTC14_EVICTION) != 0;
        tf->supports_vacancy_events = (caps & OFPTC14_VACANCY_EVENTS) != 0;
    } else {
        tf->supports_eviction = -1;
        tf->supports_vacancy_events = -1;
    }
    tf->max_entries = ntohl(otf->max_entries);

    struct ofpbuf properties = ofpbuf_const_initializer(ofpbuf_pull(msg, len),
                                                        len);
    ofpbuf_pull(&properties, sizeof *otf);
    while (properties.size > 0) {
        struct ofpbuf payload;
        enum ofperr error;
        uint64_t type;

        error = pull_table_feature_property(&properties, &payload, &type);
        if (error) {
            return error;
        }

        switch ((enum ofp13_table_feature_prop_type) type) {
        case OFPTFPT13_INSTRUCTIONS:
            error = parse_instruction_ids(&payload, loose,
                                          &tf->nonmiss.instructions);
            break;
        case OFPTFPT13_INSTRUCTIONS_MISS:
            error = parse_instruction_ids(&payload, loose,
                                          &tf->miss.instructions);
            break;

        case OFPTFPT13_NEXT_TABLES:
            error = parse_table_features_next_table(&payload,
                                                    tf->nonmiss.next);
            break;
        case OFPTFPT13_NEXT_TABLES_MISS:
            error = parse_table_features_next_table(&payload, tf->miss.next);
            break;

        case OFPTFPT13_WRITE_ACTIONS:
            error = parse_action_bitmap(&payload, oh->version,
                                        &tf->nonmiss.write.ofpacts);
            break;
        case OFPTFPT13_WRITE_ACTIONS_MISS:
            error = parse_action_bitmap(&payload, oh->version,
                                        &tf->miss.write.ofpacts);
            break;

        case OFPTFPT13_APPLY_ACTIONS:
            error = parse_action_bitmap(&payload, oh->version,
                                        &tf->nonmiss.apply.ofpacts);
            break;
        case OFPTFPT13_APPLY_ACTIONS_MISS:
            error = parse_action_bitmap(&payload, oh->version,
                                        &tf->miss.apply.ofpacts);
            break;

        case OFPTFPT13_MATCH:
            error = parse_oxms(&payload, loose, &tf->match, &tf->mask);
            break;
        case OFPTFPT13_WILDCARDS:
            error = parse_oxms(&payload, loose, &tf->wildcard, NULL);
            break;

        case OFPTFPT13_WRITE_SETFIELD:
            error = parse_oxms(&payload, loose,
                               &tf->nonmiss.write.set_fields, NULL);
            break;
        case OFPTFPT13_WRITE_SETFIELD_MISS:
            error = parse_oxms(&payload, loose,
                               &tf->miss.write.set_fields, NULL);
            break;
        case OFPTFPT13_APPLY_SETFIELD:
            error = parse_oxms(&payload, loose,
                               &tf->nonmiss.apply.set_fields, NULL);
            break;
        case OFPTFPT13_APPLY_SETFIELD_MISS:
            error = parse_oxms(&payload, loose,
                               &tf->miss.apply.set_fields, NULL);
            break;

        case OFPTFPT13_EXPERIMENTER:
        case OFPTFPT13_EXPERIMENTER_MISS:
        default:
            error = OFPPROP_UNKNOWN(loose, "table features", type);
            break;
        }
        if (error) {
            return error;
        }
    }

    /* Fix inconsistencies:
     *
     *     - Turn on 'match' bits that are set in 'mask', because maskable
     *       fields are matchable.
     *
     *     - Turn on 'wildcard' bits that are set in 'mask', because a field
     *       that is arbitrarily maskable can be wildcarded entirely.
     *
     *     - Turn off 'wildcard' bits that are not in 'match', because a field
     *       must be matchable for it to be meaningfully wildcarded. */
    bitmap_or(tf->match.bm, tf->mask.bm, MFF_N_IDS);
    bitmap_or(tf->wildcard.bm, tf->mask.bm, MFF_N_IDS);
    bitmap_and(tf->wildcard.bm, tf->match.bm, MFF_N_IDS);

    return 0;
}
