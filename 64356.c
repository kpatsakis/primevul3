ofproto_port_set_queues(struct ofproto *ofproto, ofp_port_t ofp_port,
                        const struct ofproto_port_queue *queues,
                        size_t n_queues)
{
    struct ofport *ofport = ofproto_get_port(ofproto, ofp_port);

    if (!ofport) {
        VLOG_WARN("%s: cannot set queues on nonexistent port %"PRIu32,
                  ofproto->name, ofp_port);
        return ENODEV;
    }

    return (ofproto->ofproto_class->set_queues
            ? ofproto->ofproto_class->set_queues(ofport, queues, n_queues)
            : EOPNOTSUPP);
}
