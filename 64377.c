ofproto_set_rstp(struct ofproto *ofproto,
                 const struct ofproto_rstp_settings *s)
{
    if (!ofproto->ofproto_class->set_rstp) {
        return EOPNOTSUPP;
    }
    ofproto->ofproto_class->set_rstp(ofproto, s);
    return 0;
}
