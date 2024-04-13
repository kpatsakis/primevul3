handle_queue_stats_for_port(struct ofport *port, uint32_t queue_id,
                            struct queue_stats_cbdata *cbdata)
{
    cbdata->ofport = port;
    if (queue_id == OFPQ_ALL) {
        netdev_dump_queue_stats(port->netdev,
                                handle_queue_stats_dump_cb, cbdata);
    } else {
        struct netdev_queue_stats stats;

        if (!netdev_get_queue_stats(port->netdev, queue_id, &stats)) {
            put_queue_stats(cbdata, queue_id, &stats);
        } else {
            return OFPERR_OFPQOFC_BAD_QUEUE;
        }
    }
    return 0;
}
