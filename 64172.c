    OVS_REQUIRES(ofproto_mutex)
{
    struct rule *rule = ofm->temp_rule;

    /* ofproto_flow_mod_start() consumes the reference, so we
     * take a new one. */
    ofproto_rule_ref(rule);
    enum ofperr error = ofproto_flow_mod_start(rule->ofproto, ofm);
    ofm->temp_rule = rule;

    return error;
}
