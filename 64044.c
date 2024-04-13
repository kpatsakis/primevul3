ofpacts_parse_actions(const char *s, struct ofpbuf *ofpacts,
                      enum ofputil_protocol *usable_protocols)
{
    return ofpacts_parse_copy(s, ofpacts, usable_protocols, false, 0);
}
