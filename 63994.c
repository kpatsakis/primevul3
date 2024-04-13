format_WRITE_ACTIONS(const struct ofpact_nest *a, struct ds *s)
{
    ds_put_format(s, "%swrite_actions(%s", colors.paren, colors.end);
    ofpacts_format(a->actions, ofpact_nest_get_action_len(a), s);
    ds_put_format(s, "%s)%s", colors.paren, colors.end);
}
