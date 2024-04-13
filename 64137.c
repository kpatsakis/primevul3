    OVS_EXCLUDED(ofproto_mutex)
{
    struct oftable *table;
    struct cls_rule cr;
    uint8_t table_id;
    enum ofperr error;

    if (!check_table_id(ofproto, fm->table_id)) {
        return OFPERR_OFPBRC_BAD_TABLE_ID;
    }

    /* Pick table. */
    if (fm->table_id == 0xff) {
        if (ofproto->ofproto_class->rule_choose_table) {
            error = ofproto->ofproto_class->rule_choose_table(ofproto,
                                                              &fm->match,
                                                              &table_id);
            if (error) {
                return error;
            }
            ovs_assert(table_id < ofproto->n_tables);
        } else {
            table_id = 0;
        }
    } else if (fm->table_id < ofproto->n_tables) {
        table_id = fm->table_id;
    } else {
        return OFPERR_OFPBRC_BAD_TABLE_ID;
    }

    table = &ofproto->tables[table_id];
    if (table->flags & OFTABLE_READONLY
        && !(fm->flags & OFPUTIL_FF_NO_READONLY)) {
        return OFPERR_OFPBRC_EPERM;
    }

    if (!(fm->flags & OFPUTIL_FF_HIDDEN_FIELDS)
        && !match_has_default_hidden_fields(&fm->match)) {
        VLOG_WARN_RL(&rl, "%s: (add_flow) only internal flows can set "
                     "non-default values to hidden fields", ofproto->name);
        return OFPERR_OFPBRC_EPERM;
    }

    if (!ofm->temp_rule) {
        cls_rule_init(&cr, &fm->match, fm->priority);

        /* Allocate new rule.  Destroys 'cr'. */
        error = ofproto_rule_create(ofproto, &cr, table - ofproto->tables,
                                    fm->new_cookie, fm->idle_timeout,
                                    fm->hard_timeout, fm->flags,
                                    fm->importance, fm->ofpacts,
                                    fm->ofpacts_len,
                                    fm->match.flow.tunnel.metadata.present.map,
                                    fm->ofpacts_tlv_bitmap, &ofm->temp_rule);
        if (error) {
            return error;
        }

        get_conjunctions(fm, &ofm->conjs, &ofm->n_conjs);
    }
    return 0;
}
