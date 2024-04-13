simple_flow_mod(struct ofproto *ofproto,
                const struct match *match, int priority,
                const struct ofpact *ofpacts, size_t ofpacts_len,
                enum ofp_flow_mod_command command)
{
    struct ofputil_flow_mod fm;

    flow_mod_init(&fm, match, priority, ofpacts, ofpacts_len, command);

    return handle_flow_mod__(ofproto, &fm, NULL);
}
