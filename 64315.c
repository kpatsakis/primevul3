ofproto_get_rstp_status(struct ofproto *ofproto,
                        struct ofproto_rstp_status *s)
{
    if (!ofproto->ofproto_class->get_rstp_status) {
        return EOPNOTSUPP;
    }
    ofproto->ofproto_class->get_rstp_status(ofproto, s);
    return 0;
}
