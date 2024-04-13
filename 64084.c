parse_SET_FIELD(const char *arg, struct ofpbuf *ofpacts,
                enum ofputil_protocol *usable_protocols)
{
    char *copy = xstrdup(arg);
    char *error = set_field_parse__(copy, ofpacts, usable_protocols);
    free(copy);
    return error;
}
