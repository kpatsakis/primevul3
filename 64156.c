    OVS_REQUIRES(ofproto_mutex)
{
    if (group_id == OFPG_ANY) {
        return true;
    } else {
        const struct rule_actions *actions = rule_get_actions(rule);
        return ofpacts_output_to_group(actions->ofpacts,
                                       actions->ofpacts_len, group_id);
    }
}
