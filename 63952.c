format_CONJUNCTION(const struct ofpact_conjunction *oc, struct ds *s)
{
    ds_put_format(s, "%sconjunction(%s%"PRIu32",%"PRIu8"/%"PRIu8"%s)%s",
                  colors.paren, colors.end,
                  oc->id, oc->clause + 1, oc->n_clauses,
                  colors.paren, colors.end);
}
