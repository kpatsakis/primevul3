    OVS_EXCLUDED(ofproto_mutex)
{
    struct oftable *table;

    /* This will flush all datapath flows. */
    if (ofproto->ofproto_class->flush) {
        ofproto->ofproto_class->flush(ofproto);
    }

    /* XXX: There is a small race window here, where new datapath flows can be
     * created by upcall handlers based on the existing flow table.  We can not
     * call ofproto class flush while holding 'ofproto_mutex' to prevent this,
     * as then we could deadlock on syncing with the handler threads waiting on
     * the same mutex. */

    ovs_mutex_lock(&ofproto_mutex);
    OFPROTO_FOR_EACH_TABLE (table, ofproto) {
        struct rule_collection rules;
        struct rule *rule;

        if (table->flags & OFTABLE_HIDDEN) {
            continue;
        }

        rule_collection_init(&rules);

        CLS_FOR_EACH (rule, cr, &table->cls) {
            rule_collection_add(&rules, rule);
        }
        delete_flows__(&rules, OFPRR_DELETE, NULL);
    }
    /* XXX: Concurrent handler threads may insert new learned flows based on
     * learn actions of the now deleted flows right after we release
     * 'ofproto_mutex'. */
    ovs_mutex_unlock(&ofproto_mutex);
}
