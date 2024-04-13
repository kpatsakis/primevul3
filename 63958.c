format_ENQUEUE(const struct ofpact_enqueue *a, struct ds *s)
{
    ds_put_format(s, "%senqueue:%s", colors.param, colors.end);
    ofputil_format_port(a->port, s);
    ds_put_format(s, ":%"PRIu32, a->queue);
}
