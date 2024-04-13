ofproto_aa_vlan_get_queued(struct ofproto *ofproto,
                           struct ovs_list *list)
{
    if (!ofproto->ofproto_class->aa_vlan_get_queued) {
        return EOPNOTSUPP;
    }
    ofproto->ofproto_class->aa_vlan_get_queued(ofproto, list);
    return 0;
}
