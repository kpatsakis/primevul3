decode_OFPAT_RAW11_SET_VLAN_PCP(uint8_t pcp,
                                enum ofp_version ofp_version OVS_UNUSED,
                                struct ofpbuf *out)
{
    return decode_set_vlan_pcp(pcp, false, out);
}
