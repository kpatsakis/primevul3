parse_POP_QUEUE(const char *arg OVS_UNUSED, struct ofpbuf *ofpacts,
                enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    ofpact_put_POP_QUEUE(ofpacts);
    return NULL;
}
