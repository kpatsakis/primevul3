ofproto_get_ipfix_stats(struct ofproto *ofproto,
                        bool bridge_ipfix,
                        struct ovs_list *replies)
{
    int error;

    if (ofproto->ofproto_class->get_ipfix_stats) {
        error = ofproto->ofproto_class->get_ipfix_stats(ofproto,
                                                          bridge_ipfix,
                                                          replies);
    } else {
        error = EOPNOTSUPP;
    }

    return error;
}
