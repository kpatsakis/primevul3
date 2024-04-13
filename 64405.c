rule_actions_create(const struct ofpact *ofpacts, size_t ofpacts_len)
{
    struct rule_actions *actions;

    actions = xmalloc(sizeof *actions + ofpacts_len);
    actions->ofpacts_len = ofpacts_len;
    memcpy(actions->ofpacts, ofpacts, ofpacts_len);
    actions->has_meter = ofpacts_get_meter(ofpacts, ofpacts_len) != 0;
    actions->has_groups =
        (ofpact_find_type_flattened(ofpacts, OFPACT_GROUP,
                                    ofpact_end(ofpacts, ofpacts_len))
         != NULL);
    actions->has_learn_with_delete = (next_learn_with_delete(actions, NULL)
                                      != NULL);

    return actions;
}
