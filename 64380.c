ofproto_set_stp(struct ofproto *ofproto,
                const struct ofproto_stp_settings *s)
{
    return (ofproto->ofproto_class->set_stp
            ? ofproto->ofproto_class->set_stp(ofproto, s)
            : EOPNOTSUPP);
}
