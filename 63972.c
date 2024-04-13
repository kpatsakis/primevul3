format_PUSH_MPLS(const struct ofpact_push_mpls *a, struct ds *s)
{
    ds_put_format(s, "%spush_mpls:%s0x%04"PRIx16,
                  colors.param, colors.end, ntohs(a->ethertype));
}
