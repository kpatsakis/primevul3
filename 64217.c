append_port_stat(struct ofport *port, struct ovs_list *replies)
{
    struct ofputil_port_stats ops = { .port_no = port->pp.port_no };

    calc_duration(port->created, time_msec(),
                  &ops.duration_sec, &ops.duration_nsec);

    /* Intentionally ignore return value, since errors will set
     * 'stats' to all-1s, which is correct for OpenFlow, and
     * netdev_get_stats() will log errors. */
    ofproto_port_get_stats(port, &ops.stats);

    ofputil_append_port_stat(replies, &ops);
}
