    OVS_EXCLUDED(ofproto_mutex)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);

    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));

    struct ofmonitor **monitors = NULL;
    size_t allocated_monitors = 0;
    size_t n_monitors = 0;

    enum ofperr error;

    ovs_mutex_lock(&ofproto_mutex);
    for (;;) {
        struct ofputil_flow_monitor_request request;
        struct ofmonitor *m;
        int retval;

        retval = ofputil_decode_flow_monitor_request(&request, &b);
        if (retval == EOF) {
            break;
        } else if (retval) {
            error = retval;
            goto error;
        }

        if (request.table_id != 0xff
            && request.table_id >= ofproto->n_tables) {
            error = OFPERR_OFPBRC_BAD_TABLE_ID;
            goto error;
        }

        error = ofmonitor_create(&request, ofconn, &m);
        if (error) {
            goto error;
        }

        if (n_monitors >= allocated_monitors) {
            monitors = x2nrealloc(monitors, &allocated_monitors,
                                  sizeof *monitors);
        }
        monitors[n_monitors++] = m;
    }

    struct rule_collection rules;
    rule_collection_init(&rules);
    for (size_t i = 0; i < n_monitors; i++) {
        ofproto_collect_ofmonitor_initial_rules(monitors[i], &rules);
    }

    struct ovs_list replies;
    ofpmp_init(&replies, oh);
    ofmonitor_compose_refresh_updates(&rules, &replies);
    ovs_mutex_unlock(&ofproto_mutex);

    rule_collection_destroy(&rules);

    ofconn_send_replies(ofconn, &replies);
    free(monitors);

    return 0;

error:
    for (size_t i = 0; i < n_monitors; i++) {
        ofmonitor_destroy(monitors[i]);
    }
    free(monitors);
    ovs_mutex_unlock(&ofproto_mutex);

    return error;
}
