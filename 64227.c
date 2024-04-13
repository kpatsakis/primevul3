flow_stats_ds(struct ofproto *ofproto, struct rule *rule, struct ds *results)
{
    uint64_t packet_count, byte_count;
    const struct rule_actions *actions;
    long long int created, used;

    rule->ofproto->ofproto_class->rule_get_stats(rule, &packet_count,
                                                 &byte_count, &used);

    ovs_mutex_lock(&rule->mutex);
    actions = rule_get_actions(rule);
    created = rule->created;
    ovs_mutex_unlock(&rule->mutex);

    if (rule->table_id != 0) {
        ds_put_format(results, "table_id=%"PRIu8", ", rule->table_id);
    }
    ds_put_format(results, "duration=%llds, ", (time_msec() - created) / 1000);
    ds_put_format(results, "n_packets=%"PRIu64", ", packet_count);
    ds_put_format(results, "n_bytes=%"PRIu64", ", byte_count);
    cls_rule_format(&rule->cr, ofproto_get_tun_tab(ofproto), results);
    ds_put_char(results, ',');

    ds_put_cstr(results, "actions=");
    ofpacts_format(actions->ofpacts, actions->ofpacts_len, results);

    ds_put_cstr(results, "\n");
}
