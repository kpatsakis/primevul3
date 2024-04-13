ofproto_set_ipfix(struct ofproto *ofproto,
                  const struct ofproto_ipfix_bridge_exporter_options *bo,
                  const struct ofproto_ipfix_flow_exporter_options *fo,
                  size_t n_fo)
{
    if (ofproto->ofproto_class->set_ipfix) {
        return ofproto->ofproto_class->set_ipfix(ofproto, bo, fo, n_fo);
    } else {
        return (bo || fo) ? EOPNOTSUPP : 0;
    }
}
