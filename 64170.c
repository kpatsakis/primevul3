    OVS_REQUIRES(ofproto_mutex)
{
    struct rule *old_rule = NULL;
    struct rule *new_rule = ofm->temp_rule;
    const struct rule_actions *actions = rule_get_actions(new_rule);
    struct oftable *table = &ofproto->tables[new_rule->table_id];
    enum ofperr error;

    /* Must check actions while holding ofproto_mutex to avoid a race. */
    error = ofproto_check_ofpacts(ofproto, actions->ofpacts,
                                  actions->ofpacts_len);
    if (error) {
        return error;
    }

    /* Check for the existence of an identical rule.
     * This will not return rules earlier marked for removal. */
    old_rule = rule_from_cls_rule(classifier_find_rule_exactly(&table->cls,
                                                               &new_rule->cr,
                                                               ofm->version));
    if (!old_rule) {
        /* Check for overlap, if requested. */
        if (new_rule->flags & OFPUTIL_FF_CHECK_OVERLAP
            && classifier_rule_overlaps(&table->cls, &new_rule->cr,
                                        ofm->version)) {
            return OFPERR_OFPFMFC_OVERLAP;
        }

        /* If necessary, evict an existing rule to clear out space. */
        if (table->n_flows >= table->max_flows) {
            if (!choose_rule_to_evict(table, &old_rule)) {
                return OFPERR_OFPFMFC_TABLE_FULL;
            }
            eviction_group_remove_rule(old_rule);
            /* Marks 'old_rule' as an evicted rule rather than replaced rule.
             */
            old_rule->removed_reason = OFPRR_EVICTION;
        }
    } else {
        ofm->modify_cookie = true;
    }

    if (old_rule) {
        rule_collection_add(&ofm->old_rules, old_rule);
    }
    /* Take ownership of the temp_rule. */
    rule_collection_add(&ofm->new_rules, new_rule);
    ofm->temp_rule = NULL;

    replace_rule_start(ofproto, ofm, old_rule, new_rule);
    return 0;
}
