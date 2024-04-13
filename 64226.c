flow_mod_init(struct ofputil_flow_mod *fm,
              const struct match *match, int priority,
              const struct ofpact *ofpacts, size_t ofpacts_len,
              enum ofp_flow_mod_command command)
{
    *fm = (struct ofputil_flow_mod) {
        .match = *match,
        .priority = priority,
        .table_id = 0,
        .command = command,
        .buffer_id = UINT32_MAX,
        .out_port = OFPP_ANY,
        .out_group = OFPG_ANY,
        .ofpacts = CONST_CAST(struct ofpact *, ofpacts),
        .ofpacts_len = ofpacts_len,
    };
}
