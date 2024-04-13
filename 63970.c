format_POP_MPLS(const struct ofpact_pop_mpls *a, struct ds *s)
{
    ds_put_format(s, "%spop_mpls:%s0x%04"PRIx16,
                  colors.param, colors.end, ntohs(a->ethertype));
}
