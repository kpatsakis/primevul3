ofproto_set_netflow(struct ofproto *ofproto,
                    const struct netflow_options *nf_options)
{
    if (nf_options && sset_is_empty(&nf_options->collectors)) {
        nf_options = NULL;
    }

    if (ofproto->ofproto_class->set_netflow) {
        return ofproto->ofproto_class->set_netflow(ofproto, nf_options);
    } else {
        return nf_options ? EOPNOTSUPP : 0;
    }
}
