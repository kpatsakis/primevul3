ofproto_rule_unref(struct rule *rule)
{
    if (rule && ovs_refcount_unref_relaxed(&rule->ref_count) == 1) {
        ovs_assert(rule->state != RULE_INSERTED);
        ovsrcu_postpone(rule_destroy_cb, rule);
    }
}
