    OVS_EXCLUDED(ofproto_mutex)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct ofputil_flow_stats_request fsr;
    struct rule_criteria criteria;
    struct rule_collection rules;
    struct ovs_list replies;
    enum ofperr error;

    error = ofputil_decode_flow_stats_request(&fsr, request,
                                              ofproto_get_tun_tab(ofproto),
                                              &ofproto->vl_mff_map);
    if (error) {
        return error;
    }

    rule_criteria_init(&criteria, fsr.table_id, &fsr.match, 0, OVS_VERSION_MAX,
                       fsr.cookie, fsr.cookie_mask, fsr.out_port,
                       fsr.out_group);

    ovs_mutex_lock(&ofproto_mutex);
    error = collect_rules_loose(ofproto, &criteria, &rules);
    rule_criteria_destroy(&criteria);
    if (!error) {
        rule_collection_ref(&rules);
    }
    ovs_mutex_unlock(&ofproto_mutex);

    if (error) {
        return error;
    }

    ofpmp_init(&replies, request);
    struct rule *rule;
    RULE_COLLECTION_FOR_EACH (rule, &rules) {
        long long int now = time_msec();
        struct ofputil_flow_stats fs;
        long long int created, used, modified;
        const struct rule_actions *actions;
        enum ofputil_flow_mod_flags flags;

        ovs_mutex_lock(&rule->mutex);
        fs.cookie = rule->flow_cookie;
        fs.idle_timeout = rule->idle_timeout;
        fs.hard_timeout = rule->hard_timeout;
        fs.importance = rule->importance;
        created = rule->created;
        modified = rule->modified;
        actions = rule_get_actions(rule);
        flags = rule->flags;
        ovs_mutex_unlock(&rule->mutex);

        ofproto->ofproto_class->rule_get_stats(rule, &fs.packet_count,
                                               &fs.byte_count, &used);

        minimatch_expand(&rule->cr.match, &fs.match);
        fs.table_id = rule->table_id;
        calc_duration(created, now, &fs.duration_sec, &fs.duration_nsec);
        fs.priority = rule->cr.priority;
        fs.idle_age = age_secs(now - used);
        fs.hard_age = age_secs(now - modified);
        fs.ofpacts = actions->ofpacts;
        fs.ofpacts_len = actions->ofpacts_len;

        fs.flags = flags;
        ofputil_append_flow_stats_reply(&fs, &replies,
                                        ofproto_get_tun_tab(ofproto));
    }

    rule_collection_unref(&rules);
    rule_collection_destroy(&rules);

    ofconn_send_replies(ofconn, &replies);

    return 0;
}
