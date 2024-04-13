    OVS_EXCLUDED(ofproto_mutex)
{
    const struct rule *rule;
    bool must_add;

    /* First do a cheap check whether the rule we're looking for already exists
     * with the actions that we want.  If it does, then we're done. */
    rule = rule_from_cls_rule(classifier_find_match_exactly(
                                  &ofproto->tables[0].cls, match, priority,
                                  OVS_VERSION_MAX));
    if (rule) {
        const struct rule_actions *actions = rule_get_actions(rule);
        must_add = !ofpacts_equal(actions->ofpacts, actions->ofpacts_len,
                                  ofpacts, ofpacts_len);
    } else {
        must_add = true;
    }

    /* If there's no such rule or the rule doesn't have the actions we want,
     * fall back to a executing a full flow mod.  We can't optimize this at
     * all because we didn't take enough locks above to ensure that the flow
     * table didn't already change beneath us.  */
    if (must_add) {
        simple_flow_mod(ofproto, match, priority, ofpacts, ofpacts_len,
                        OFPFC_MODIFY_STRICT);
    }
}
