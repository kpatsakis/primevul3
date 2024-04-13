format_PUSH_VLAN(const struct ofpact_null *a OVS_UNUSED, struct ds *s)
{
    /* XXX 802.1AD case*/
    ds_put_format(s, "%spush_vlan:%s%#"PRIx16,
                  colors.param, colors.end, ETH_TYPE_VLAN_8021Q);
}
