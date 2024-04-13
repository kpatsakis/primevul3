ofproto_get_all_flows(struct ofproto *p, struct ds *results)
{
    struct oftable *table;

    OFPROTO_FOR_EACH_TABLE (table, p) {
        struct rule *rule;

        CLS_FOR_EACH (rule, cr, &table->cls) {
            flow_stats_ds(p, rule, results);
        }
    }
}
