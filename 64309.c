ofproto_get_memory_usage(const struct ofproto *ofproto, struct simap *usage)
{
    const struct oftable *table;
    unsigned int n_rules;

    simap_increase(usage, "ports", hmap_count(&ofproto->ports));

    n_rules = 0;
    OFPROTO_FOR_EACH_TABLE (table, ofproto) {
        n_rules += table->n_flows;
    }
    simap_increase(usage, "rules", n_rules);

    if (ofproto->ofproto_class->get_memory_usage) {
        ofproto->ofproto_class->get_memory_usage(ofproto, usage);
    }

    connmgr_get_memory_usage(ofproto->connmgr, usage);
}
