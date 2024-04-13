parse_CT(char *arg, struct ofpbuf *ofpacts,
         enum ofputil_protocol *usable_protocols)
{
    const size_t ct_offset = ofpacts_pull(ofpacts);
    struct ofpact_conntrack *oc;
    char *error = NULL;
    char *key, *value;

    oc = ofpact_put_CT(ofpacts);
    oc->flags = 0;
    oc->recirc_table = NX_CT_RECIRC_NONE;
    while (ofputil_parse_key_value(&arg, &key, &value)) {
        if (!strcmp(key, "commit")) {
            oc->flags |= NX_CT_F_COMMIT;
        } else if (!strcmp(key, "table")) {
            error = str_to_u8(value, "recirc_table", &oc->recirc_table);
            if (!error && oc->recirc_table == NX_CT_RECIRC_NONE) {
                error = xasprintf("invalid table %#"PRIx16, oc->recirc_table);
            }
        } else if (!strcmp(key, "zone")) {
            error = str_to_u16(value, "zone", &oc->zone_imm);

            if (error) {
                free(error);
                error = mf_parse_subfield(&oc->zone_src, value);
                if (error) {
                    return error;
                }
            }
        } else if (!strcmp(key, "alg")) {
            error = str_to_connhelper(value, &oc->alg);
        } else if (!strcmp(key, "nat")) {
            const size_t nat_offset = ofpacts_pull(ofpacts);

            error = parse_NAT(value, ofpacts, usable_protocols);
            /* Update CT action pointer and length. */
            ofpacts->header = ofpbuf_push_uninit(ofpacts, nat_offset);
            oc = ofpacts->header;
        } else if (!strcmp(key, "exec")) {
            /* Hide existing actions from ofpacts_parse_copy(), so the
             * nesting can be handled transparently. */
            enum ofputil_protocol usable_protocols2;
            const size_t exec_offset = ofpacts_pull(ofpacts);

            /* Initializes 'usable_protocol2', fold it back to
             * '*usable_protocols' afterwards, so that we do not lose
             * restrictions already in there. */
            error = ofpacts_parse_copy(value, ofpacts, &usable_protocols2,
                                       false, OFPACT_CT);
            *usable_protocols &= usable_protocols2;
            ofpacts->header = ofpbuf_push_uninit(ofpacts, exec_offset);
            oc = ofpacts->header;
        } else {
            error = xasprintf("invalid argument to \"ct\" action: `%s'", key);
        }
        if (error) {
            break;
        }
    }

    ofpact_finish_CT(ofpacts, &oc);
    ofpbuf_push_uninit(ofpacts, ct_offset);
    return error;
}
