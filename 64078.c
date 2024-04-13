parse_OUTPUT_TRUNC(const char *arg, struct ofpbuf *ofpacts OVS_UNUSED,
                 enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    /* Disable output_trunc parsing.  Expose as output(port=N,max_len=M) and
     * reuse parse_OUTPUT to parse output_trunc action. */
    return xasprintf("unknown action %s", arg);
}
