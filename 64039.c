ofpacts_execute_action_set(struct ofpbuf *action_list,
                           const struct ofpbuf *action_set)
{
    /* The OpenFlow spec "Action Set" section specifies this order. */
    ofpacts_copy_last(action_list, action_set, OFPACT_STRIP_VLAN);
    ofpacts_copy_last(action_list, action_set, OFPACT_POP_MPLS);
    ofpacts_copy_last(action_list, action_set, OFPACT_PUSH_MPLS);
    ofpacts_copy_last(action_list, action_set, OFPACT_PUSH_VLAN);
    ofpacts_copy_last(action_list, action_set, OFPACT_DEC_TTL);
    ofpacts_copy_last(action_list, action_set, OFPACT_DEC_MPLS_TTL);
    ofpacts_copy_all(action_list, action_set, ofpact_is_set_or_move_action);
    ofpacts_copy_last(action_list, action_set, OFPACT_SET_QUEUE);

    /* If both OFPACT_GROUP and OFPACT_OUTPUT are present, OpenFlow says that
     * we should execute only OFPACT_GROUP.
     *
     * If neither OFPACT_GROUP nor OFPACT_OUTPUT is present, then we can drop
     * all the actions because there's no point in modifying a packet that will
     * not be sent anywhere. */
    if (!ofpacts_copy_last(action_list, action_set, OFPACT_GROUP) &&
        !ofpacts_copy_last(action_list, action_set, OFPACT_OUTPUT) &&
        !ofpacts_copy_last(action_list, action_set, OFPACT_RESUBMIT) &&
        !ofpacts_copy_last(action_list, action_set, OFPACT_CT)) {
        ofpbuf_clear(action_list);
    }
}
