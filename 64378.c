ofproto_set_sflow(struct ofproto *ofproto,
                  const struct ofproto_sflow_options *oso)
{
    if (oso && sset_is_empty(&oso->targets)) {
        oso = NULL;
    }

    if (ofproto->ofproto_class->set_sflow) {
        return ofproto->ofproto_class->set_sflow(ofproto, oso);
    } else {
        return oso ? EOPNOTSUPP : 0;
    }
}
