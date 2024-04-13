    OVS_REQUIRES(ofproto_mutex)
{
    if (port == OFPP_ANY) {
        return true;
    } else {
        const struct rule_actions *actions = rule_get_actions(rule);
        return ofpacts_output_to_port(actions->ofpacts,
                                      actions->ofpacts_len, port);
    }
}
