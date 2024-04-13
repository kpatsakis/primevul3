parse_CT_CLEAR(char *arg OVS_UNUSED, struct ofpbuf *ofpacts,
               enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    ofpact_put_CT_CLEAR(ofpacts);
    return NULL;
}
