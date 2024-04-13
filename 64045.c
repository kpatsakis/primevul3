ofpacts_parse_copy(const char *s_, struct ofpbuf *ofpacts,
                   enum ofputil_protocol *usable_protocols,
                   bool allow_instructions, enum ofpact_type outer_action)
{
    char *error, *s;

    *usable_protocols = OFPUTIL_P_ANY;

    s = xstrdup(s_);
    error = ofpacts_parse(s, ofpacts, usable_protocols, allow_instructions,
                          outer_action);
    free(s);

    return error;
}
