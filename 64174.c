    OVS_REQUIRES(ofproto_mutex)
{
    struct rule *replaced_rule;

    replaced_rule = (old_rule && old_rule->removed_reason != OFPRR_EVICTION)
        ? old_rule : NULL;

    /* Insert the new flow to the ofproto provider.  A non-NULL 'replaced_rule'
     * is a duplicate rule the 'new_rule' is replacing.  The provider should
     * link the packet and byte counts from the old rule to the new one if
     * 'modify_keep_counts' is 'true'.  The 'replaced_rule' will be deleted
     * right after this call. */
    ofproto->ofproto_class->rule_insert(new_rule, replaced_rule,
                                        ofm->modify_keep_counts);
    learned_cookies_inc(ofproto, rule_get_actions(new_rule));

    if (old_rule) {
        const struct rule_actions *old_actions = rule_get_actions(old_rule);
        const struct rule_actions *new_actions = rule_get_actions(new_rule);

        learned_cookies_dec(ofproto, old_actions, dead_cookies);

        if (replaced_rule) {
            enum nx_flow_update_event event = ofm->command == OFPFC_ADD
                ? NXFME_ADDED : NXFME_MODIFIED;

            bool changed_cookie = (new_rule->flow_cookie
                                   != old_rule->flow_cookie);

            bool changed_actions = !ofpacts_equal(new_actions->ofpacts,
                                                  new_actions->ofpacts_len,
                                                  old_actions->ofpacts,
                                                  old_actions->ofpacts_len);

            if (event != NXFME_MODIFIED || changed_actions
                || changed_cookie) {
                ofmonitor_report(ofproto->connmgr, new_rule, event, 0,
                                 req ? req->ofconn : NULL,
                                 req ? req->request->xid : 0,
                                 changed_actions ? old_actions : NULL);
            }
        } else {
            /* XXX: This is slight duplication with delete_flows_finish__() */
            ofmonitor_report(ofproto->connmgr, old_rule, NXFME_DELETED,
                             OFPRR_EVICTION,
                             req ? req->ofconn : NULL,
                             req ? req->request->xid : 0, NULL);
        }
    }
}
