parse_stack_prop(const struct ofpbuf *property, struct ofpbuf *stack)
{
    unsigned int len = ofpbuf_msgsize(property);
    if (len > sizeof(union mf_subvalue)) {
        VLOG_WARN_RL(&bad_ofmsg_rl, "NXCPT_STACK property has bad length %u",
                     len);
        return OFPERR_OFPBPC_BAD_LEN;
    }
    nx_stack_push_bottom(stack, property->msg, len);
    return 0;
}
