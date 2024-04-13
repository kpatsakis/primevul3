handle_queue_get_config_request_for_port(struct ofport *port, uint32_t queue,
                                         struct ovs_list *replies)
{
    struct smap details = SMAP_INITIALIZER(&details);
    if (queue != OFPQ_ALL) {
        int error = netdev_get_queue(port->netdev, queue, &details);
        switch (error) {
        case 0:
            put_queue_get_config_reply(port, queue, replies);
            break;
        case EOPNOTSUPP:
        case EINVAL:
            return OFPERR_OFPQOFC_BAD_QUEUE;
        default:
            return OFPERR_NXQOFC_QUEUE_ERROR;
        }
    } else {
        struct netdev_queue_dump queue_dump;
        uint32_t queue_id;

        NETDEV_QUEUE_FOR_EACH (&queue_id, &details, &queue_dump,
                               port->netdev) {
            put_queue_get_config_reply(port, queue_id, replies);
        }
    }
    smap_destroy(&details);
    return 0;
}
