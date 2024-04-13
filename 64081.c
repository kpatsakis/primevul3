parse_PUSH_VLAN(char *arg, struct ofpbuf *ofpacts,
                enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    uint16_t ethertype;
    char *error;

    *usable_protocols &= OFPUTIL_P_OF11_UP;
    error = str_to_u16(arg, "ethertype", &ethertype);
    if (error) {
        return error;
    }

    if (ethertype != ETH_TYPE_VLAN_8021Q) {
        /* XXX ETH_TYPE_VLAN_8021AD case isn't supported */
        return xasprintf("%s: not a valid VLAN ethertype", arg);
    }

    ofpact_put_PUSH_VLAN(ofpacts);
    return NULL;
}
