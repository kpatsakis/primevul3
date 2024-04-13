parse_NOTE(const char *arg, struct ofpbuf *ofpacts,
           enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    size_t start_ofs = ofpacts->size;
    ofpact_put_NOTE(ofpacts);
    arg = ofpbuf_put_hex(ofpacts, arg, NULL);
    if (arg[0]) {
        return xstrdup("bad hex digit in `note' argument");
    }
    struct ofpact_note *note = ofpbuf_at_assert(ofpacts, start_ofs,
                                                sizeof *note);
    note->length = ofpacts->size - (start_ofs + sizeof *note);
    ofpact_finish_NOTE(ofpacts, &note);
    return NULL;
}
