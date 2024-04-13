format_SET_VLAN_VID(const struct ofpact_vlan_vid *a, struct ds *s)
{
    ds_put_format(s, "%s%s:%s%"PRIu16, colors.param,
                  a->push_vlan_if_needed ? "mod_vlan_vid" : "set_vlan_vid",
                  colors.end, a->vlan_vid);
}
