    OVS_REQUIRES(ofproto_mutex)
{
    struct oftable *table;
    enum ofperr error = 0;
    size_t n_readonly = 0;

    rule_collection_init(rules);

    if (!check_table_id(ofproto, criteria->table_id)) {
        error = OFPERR_OFPBRC_BAD_TABLE_ID;
        goto exit;
    }

    if (criteria->cookie_mask == OVS_BE64_MAX) {
        struct rule *rule;

        HINDEX_FOR_EACH_WITH_HASH (rule, cookie_node,
                                   hash_cookie(criteria->cookie),
                                   &ofproto->cookies) {
            if (cls_rule_is_loose_match(&rule->cr, &criteria->cr.match)) {
                collect_rule(rule, criteria, rules, &n_readonly);
            }
        }
    } else {
        FOR_EACH_MATCHING_TABLE (table, criteria->table_id, ofproto) {
            struct rule *rule;

            CLS_FOR_EACH_TARGET (rule, cr, &table->cls, &criteria->cr,
                                 criteria->version) {
                collect_rule(rule, criteria, rules, &n_readonly);
            }
        }
    }

exit:
    if (!error && !rule_collection_n(rules) && n_readonly) {
        /* We didn't find any rules to modify.  We did find some read-only
         * rules that we're not allowed to modify, so report that. */
        error = OFPERR_OFPBRC_EPERM;
    }
    if (error) {
        rule_collection_destroy(rules);
    }
    return error;
}
