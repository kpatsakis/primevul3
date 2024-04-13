format_DEC_MPLS_TTL(const struct ofpact_null *a OVS_UNUSED, struct ds *s)
{
    ds_put_format(s, "%sdec_mpls_ttl%s", colors.value, colors.end);
}
