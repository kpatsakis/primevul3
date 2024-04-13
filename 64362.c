ofproto_run(struct ofproto *p)
{
    int error;
    uint64_t new_seq;

    error = p->ofproto_class->run(p);
    if (error && error != EAGAIN) {
        VLOG_ERR_RL(&rl, "%s: run failed (%s)", p->name, ovs_strerror(error));
    }

    /* Restore the eviction group heap invariant occasionally. */
    if (p->eviction_group_timer < time_msec()) {
        size_t i;

        p->eviction_group_timer = time_msec() + 1000;

        for (i = 0; i < p->n_tables; i++) {
            struct oftable *table = &p->tables[i];
            struct eviction_group *evg;
            struct rule *rule;

            if (!table->eviction) {
                continue;
            }

            if (table->n_flows > 100000) {
                static struct vlog_rate_limit count_rl =
                    VLOG_RATE_LIMIT_INIT(1, 1);
                VLOG_WARN_RL(&count_rl, "Table %"PRIuSIZE" has an excessive"
                             " number of rules: %d", i, table->n_flows);
            }

            ovs_mutex_lock(&ofproto_mutex);
            CLS_FOR_EACH (rule, cr, &table->cls) {
                if (rule->idle_timeout || rule->hard_timeout) {
                    if (!rule->eviction_group) {
                        eviction_group_add_rule(rule);
                    } else {
                        heap_raw_change(&rule->evg_node,
                                        rule_eviction_priority(p, rule));
                    }
                }
            }

            HEAP_FOR_EACH (evg, size_node, &table->eviction_groups_by_size) {
                heap_rebuild(&evg->rules);
            }
            ovs_mutex_unlock(&ofproto_mutex);
        }
    }

    if (p->ofproto_class->port_poll) {
        char *devname;

        while ((error = p->ofproto_class->port_poll(p, &devname)) != EAGAIN) {
            process_port_change(p, error, devname);
        }
    }

    new_seq = seq_read(connectivity_seq_get());
    if (new_seq != p->change_seq) {
        struct sset devnames;
        const char *devname;
        struct ofport *ofport;

        /* Update OpenFlow port status for any port whose netdev has changed.
         *
         * Refreshing a given 'ofport' can cause an arbitrary ofport to be
         * destroyed, so it's not safe to update ports directly from the
         * HMAP_FOR_EACH loop, or even to use HMAP_FOR_EACH_SAFE.  Instead, we
         * need this two-phase approach. */
        sset_init(&devnames);
        HMAP_FOR_EACH (ofport, hmap_node, &p->ports) {
            uint64_t port_change_seq;

            port_change_seq = netdev_get_change_seq(ofport->netdev);
            if (ofport->change_seq != port_change_seq) {
                ofport->change_seq = port_change_seq;
                sset_add(&devnames, netdev_get_name(ofport->netdev));
            }
        }
        SSET_FOR_EACH (devname, &devnames) {
            update_port(p, devname);
        }
        sset_destroy(&devnames);

        p->change_seq = new_seq;
    }

    connmgr_run(p->connmgr, handle_openflow);

    return error;
}
