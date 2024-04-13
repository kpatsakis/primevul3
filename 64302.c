ofproto_flow_mod_uninit(struct ofproto_flow_mod *ofm)
{
    if (ofm->temp_rule) {
        ofproto_rule_unref(ofm->temp_rule);
        ofm->temp_rule = NULL;
    }
    if (ofm->criteria.version != OVS_VERSION_NOT_REMOVED) {
        rule_criteria_destroy(&ofm->criteria);
    }
    if (ofm->conjs) {
        free(ofm->conjs);
        ofm->conjs = NULL;
        ofm->n_conjs = 0;
    }
}
