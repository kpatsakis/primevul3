    OVS_REQUIRES(ofproto_mutex)
{
    const struct ofproto *ofproto = ofconn_get_ofproto(m->ofconn);
    const struct oftable *table;
    struct cls_rule target;

    cls_rule_init_from_minimatch(&target, &m->match, 0);
    FOR_EACH_MATCHING_TABLE (table, m->table_id, ofproto) {
        struct rule *rule;

        CLS_FOR_EACH_TARGET (rule, cr, &table->cls, &target, OVS_VERSION_MAX) {
            ofproto_collect_ofmonitor_refresh_rule(m, rule, seqno, rules);
        }
    }
    cls_rule_destroy(&target);
}
