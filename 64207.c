    OVS_REQUIRES(ofproto_mutex)
{
    struct eviction_group *evg;

    *rulep = NULL;
    if (!table->eviction) {
        return false;
    }

    /* In the common case, the outer and inner loops here will each be entered
     * exactly once:
     *
     *   - The inner loop normally "return"s in its first iteration.  If the
     *     eviction group has any evictable rules, then it always returns in
     *     some iteration.
     *
     *   - The outer loop only iterates more than once if the largest eviction
     *     group has no evictable rules.
     *
     *   - The outer loop can exit only if table's 'max_flows' is all filled up
     *     by unevictable rules. */
    HEAP_FOR_EACH (evg, size_node, &table->eviction_groups_by_size) {
        struct rule *rule;

        HEAP_FOR_EACH (rule, evg_node, &evg->rules) {
            *rulep = rule;
            return true;
        }
    }

    return false;
}
