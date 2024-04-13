ofproto_aa_vlan_get_queue_size(struct ofproto *ofproto)
{
    if (!ofproto->ofproto_class->aa_vlan_get_queue_size) {
        return EOPNOTSUPP;
    }
    return ofproto->ofproto_class->aa_vlan_get_queue_size(ofproto);
}
