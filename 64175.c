    OVS_REQUIRES(ofproto_mutex)
{
    struct rule_collection *old_rules = &ofm->old_rules;
    struct rule_collection *new_rules = &ofm->new_rules;
    enum ofperr error;

    if (rule_collection_n(old_rules) > 0) {
        /* Create a new 'modified' rule for each old rule. */
        struct rule *old_rule, *new_rule;
        const struct rule_actions *actions = rule_get_actions(ofm->temp_rule);

        /* Must check actions while holding ofproto_mutex to avoid a race. */
        error = ofproto_check_ofpacts(ofproto, actions->ofpacts,
                                      actions->ofpacts_len);
        if (error) {
            return error;
        }

        /* Use the temp rule as the first new rule, and as the template for
         * the rest. */
        struct rule *temp = ofm->temp_rule;
        ofm->temp_rule = NULL;   /* We consume the template. */

        bool first = true;
        RULE_COLLECTION_FOR_EACH (old_rule, old_rules) {
            if (first) {
                /* The template rule's match is possibly a loose one, so it
                 * must be replaced with the old rule's match so that the new
                 * rule actually replaces the old one. */
                cls_rule_destroy(CONST_CAST(struct cls_rule *, &temp->cr));
                cls_rule_clone(CONST_CAST(struct cls_rule *, &temp->cr),
                               &old_rule->cr);
                if (temp->match_tlv_bitmap != old_rule->match_tlv_bitmap) {
                    mf_vl_mff_unref(&temp->ofproto->vl_mff_map,
                                    temp->match_tlv_bitmap);
                    temp->match_tlv_bitmap = old_rule->match_tlv_bitmap;
                    mf_vl_mff_ref(&temp->ofproto->vl_mff_map,
                                  temp->match_tlv_bitmap);
                }
                *CONST_CAST(uint8_t *, &temp->table_id) = old_rule->table_id;
                rule_collection_add(new_rules, temp);
                first = false;
            } else {
                struct cls_rule cr;
                cls_rule_clone(&cr, &old_rule->cr);
                error = ofproto_rule_create(ofproto, &cr, old_rule->table_id,
                                            temp->flow_cookie,
                                            temp->idle_timeout,
                                            temp->hard_timeout, temp->flags,
                                            temp->importance,
                                            temp->actions->ofpacts,
                                            temp->actions->ofpacts_len,
                                            old_rule->match_tlv_bitmap,
                                            temp->ofpacts_tlv_bitmap,
                                            &new_rule);
                if (!error) {
                    rule_collection_add(new_rules, new_rule);
                } else {
                    /* Return the template rule in place in the error case. */
                    ofm->temp_rule = temp;
                    rule_collection_rules(new_rules)[0] = NULL;

                    rule_collection_unref(new_rules);
                    rule_collection_destroy(new_rules);
                    return error;
                }
            }
        }
        ovs_assert(rule_collection_n(new_rules)
                   == rule_collection_n(old_rules));

        RULE_COLLECTIONS_FOR_EACH (old_rule, new_rule, old_rules, new_rules) {
            replace_rule_start(ofproto, ofm, old_rule, new_rule);
        }
    } else if (ofm->modify_may_add_flow) {
        /* No match, add a new flow, consumes 'temp'. */
        error = add_flow_start(ofproto, ofm);
    } else {
        /* No flow to modify and may not add a flow. */
        ofproto_rule_unref(ofm->temp_rule);
        ofm->temp_rule = NULL;   /* We consume the template. */
        error = 0;
    }

    return error;
}
