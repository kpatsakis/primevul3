format_OUTPUT(const struct ofpact_output *a, struct ds *s)
{
    if (ofp_to_u16(a->port) < ofp_to_u16(OFPP_MAX)) {
        ds_put_format(s, "%soutput:%s%"PRIu32,
                      colors.special, colors.end, a->port);
    } else {
        ofputil_format_port(a->port, s);
        if (a->port == OFPP_CONTROLLER) {
            ds_put_format(s, ":%"PRIu16, a->max_len);
        }
    }
}
