ofproto_flow_mod_learn_refresh(struct ofproto_flow_mod *ofm)
{
    enum ofperr error = 0;

    /* ofm->temp_rule is our reference to the learned rule.  We have a
     * reference to an existing rule, if it already was in the classifier,
     * otherwise we may have a fresh rule that we need to insert. */
    struct rule *rule = ofm->temp_rule;
    if (!rule) {
        return OFPERR_OFPFMFC_UNKNOWN;
    }

    /* Create a new rule if the current one has been removed from the
     * classifier.  We need to do this since RCU does not allow a current rule
     * to be reinserted before all threads have quiesced.
     *
     * It is possible that the rule is removed asynchronously, e.g., right
     * after we have read the 'rule->state' below.  In this case the next time
     * this function is executed the rule will be reinstated. */
    if (rule->state == RULE_REMOVED) {
        struct cls_rule cr;

        cls_rule_clone(&cr, &rule->cr);
        ovs_mutex_lock(&rule->mutex);
        error = ofproto_rule_create(rule->ofproto, &cr, rule->table_id,
                                    rule->flow_cookie,
                                    rule->idle_timeout,
                                    rule->hard_timeout, rule->flags,
                                    rule->importance,
                                    rule->actions->ofpacts,
                                    rule->actions->ofpacts_len,
                                    rule->match_tlv_bitmap,
                                    rule->ofpacts_tlv_bitmap,
                                    &ofm->temp_rule);
        ovs_mutex_unlock(&rule->mutex);
        if (!error) {
            ofproto_rule_unref(rule);   /* Release old reference. */
        }
    } else {
        /* Refresh the existing rule. */
        ovs_mutex_lock(&rule->mutex);
        rule->modified = time_msec();
        ovs_mutex_unlock(&rule->mutex);
    }
    return error;
}
