format_CT(const struct ofpact_conntrack *a, struct ds *s)
{
    ds_put_format(s, "%sct(%s", colors.paren, colors.end);
    if (a->flags & NX_CT_F_COMMIT) {
        ds_put_format(s, "%scommit%s,", colors.value, colors.end);
    }
    if (a->recirc_table != NX_CT_RECIRC_NONE) {
        ds_put_format(s, "%stable=%s%"PRIu8",",
                      colors.special, colors.end, a->recirc_table);
    }
    if (a->zone_src.field) {
        ds_put_format(s, "%szone=%s", colors.param, colors.end);
        mf_format_subfield(&a->zone_src, s);
        ds_put_char(s, ',');
    } else if (a->zone_imm) {
        ds_put_format(s, "%szone=%s%"PRIu16",",
                      colors.param, colors.end, a->zone_imm);
    }
    /* If the first action is a NAT action, format it outside of the 'exec'
     * envelope. */
    const struct ofpact *action = a->actions;
    size_t actions_len = ofpact_ct_get_action_len(a);
    if (actions_len && action->type == OFPACT_NAT) {
        format_NAT(ofpact_get_NAT(action), s);
        ds_put_char(s, ',');
        actions_len -= OFPACT_ALIGN(action->len);
        action = ofpact_next(action);
    }
    if (actions_len) {
        ds_put_format(s, "%sexec(%s", colors.paren, colors.end);
        ofpacts_format(action, actions_len, s);
        ds_put_format(s, "%s),%s", colors.paren, colors.end);
    }
    format_alg(a->alg, s);
    ds_chomp(s, ',');
    ds_put_format(s, "%s)%s", colors.paren, colors.end);
}
