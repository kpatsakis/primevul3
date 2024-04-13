put_actions_property(struct ofpbuf *msg, uint64_t prop_type,
                     enum ofp_version version,
                     const struct ofpact *actions, size_t actions_len)
{
    if (actions_len) {
        size_t start = ofpprop_start_nested(msg, prop_type);
        ofpacts_put_openflow_actions(actions, actions_len, msg, version);
        ofpprop_end(msg, start);
    }
}
