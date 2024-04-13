parse_METER(char *arg, struct ofpbuf *ofpacts,
            enum ofputil_protocol *usable_protocols)
{
    *usable_protocols &= OFPUTIL_P_OF13_UP;
    return str_to_u32(arg, &ofpact_put_METER(ofpacts)->meter_id);
}
