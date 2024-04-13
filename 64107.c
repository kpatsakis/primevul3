parse_set_tunnel(char *arg, struct ofpbuf *ofpacts,
                 enum ofp_raw_action_type raw)
{
    struct ofpact_tunnel *tunnel;

    tunnel = ofpact_put_SET_TUNNEL(ofpacts);
    tunnel->ofpact.raw = raw;
    return str_to_u64(arg, &tunnel->tun_id);
}
