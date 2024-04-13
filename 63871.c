decode_OFPAT_RAW11_PUSH_VLAN(ovs_be16 eth_type,
                             enum ofp_version ofp_version OVS_UNUSED,
                             struct ofpbuf *out)
{
    if (eth_type != htons(ETH_TYPE_VLAN_8021Q)) {
        /* XXX 802.1AD(QinQ) isn't supported at the moment */
        return OFPERR_OFPBAC_BAD_ARGUMENT;
    }
    ofpact_put_PUSH_VLAN(out);
    return 0;
}
