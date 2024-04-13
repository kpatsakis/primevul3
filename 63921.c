encode_PUSH_VLAN(const struct ofpact_null *null OVS_UNUSED,
                 enum ofp_version ofp_version, struct ofpbuf *out)
{
    if (ofp_version == OFP10_VERSION) {
        /* PUSH is a side effect of a SET_VLAN_VID/PCP, which should
         * follow this action. */
    } else {
        /* XXX ETH_TYPE_VLAN_8021AD case */
        put_OFPAT11_PUSH_VLAN(out, htons(ETH_TYPE_VLAN_8021Q));
    }
}
