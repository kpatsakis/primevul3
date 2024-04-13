parse_SET_QUEUE(char *arg, struct ofpbuf *ofpacts,
                enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    return str_to_u32(arg, &ofpact_put_SET_QUEUE(ofpacts)->queue_id);
}
