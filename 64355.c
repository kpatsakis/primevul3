ofproto_port_set_mcast_snooping(struct ofproto *ofproto, void *aux,
                           const struct ofproto_mcast_snooping_port_settings *s)
{
    return (ofproto->ofproto_class->set_mcast_snooping_port
            ? ofproto->ofproto_class->set_mcast_snooping_port(ofproto, aux, s)
            : EOPNOTSUPP);
}
