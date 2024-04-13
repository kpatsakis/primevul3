ofproto_set_mcast_snooping(struct ofproto *ofproto,
                           const struct ofproto_mcast_snooping_settings *s)
{
    return (ofproto->ofproto_class->set_mcast_snooping
            ? ofproto->ofproto_class->set_mcast_snooping(ofproto, s)
            : EOPNOTSUPP);
}
