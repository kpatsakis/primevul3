parse_DEBUG_RECIRC(char *arg OVS_UNUSED, struct ofpbuf *ofpacts,
                   enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    ofpact_put_DEBUG_RECIRC(ofpacts);
    return NULL;
}
