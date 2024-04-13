format_CLONE(const struct ofpact_nest *a, struct ds *s)
{
    ds_put_format(s, "%sclone(%s", colors.paren, colors.end);
    ofpacts_format(a->actions, ofpact_nest_get_action_len(a), s);
    ds_put_format(s, "%s)%s", colors.paren, colors.end);
}
