format_NAT(const struct ofpact_nat *a, struct ds *ds)
{
    ds_put_format(ds, "%snat%s", colors.paren, colors.end);

    if (a->flags & (NX_NAT_F_SRC | NX_NAT_F_DST)) {
        ds_put_format(ds, "%s(%s", colors.paren, colors.end);
        ds_put_format(ds, a->flags & NX_NAT_F_SRC ? "%ssrc%s" : "%sdst%s",
                      colors.param, colors.end);

        if (a->range_af != AF_UNSPEC) {
            ds_put_format(ds, "%s=%s", colors.param, colors.end);

            if (a->range_af == AF_INET) {
                ds_put_format(ds, IP_FMT, IP_ARGS(a->range.addr.ipv4.min));

                if (a->range.addr.ipv4.max
                    && a->range.addr.ipv4.max != a->range.addr.ipv4.min) {
                    ds_put_format(ds, "-"IP_FMT,
                                  IP_ARGS(a->range.addr.ipv4.max));
                }
            } else if (a->range_af == AF_INET6) {
                ipv6_format_addr_bracket(&a->range.addr.ipv6.min, ds,
                                        a->range.proto.min);

                if (!ipv6_mask_is_any(&a->range.addr.ipv6.max)
                    && memcmp(&a->range.addr.ipv6.max, &a->range.addr.ipv6.min,
                              sizeof(struct in6_addr)) != 0) {
                    ds_put_char(ds, '-');
                    ipv6_format_addr_bracket(&a->range.addr.ipv6.max, ds,
                                            a->range.proto.min);
                }
            }
            if (a->range.proto.min) {
                ds_put_char(ds, ':');
                ds_put_format(ds, "%"PRIu16, a->range.proto.min);

                if (a->range.proto.max
                    && a->range.proto.max != a->range.proto.min) {
                    ds_put_format(ds, "-%"PRIu16, a->range.proto.max);
                }
            }
            ds_put_char(ds, ',');

            if (a->flags & NX_NAT_F_PERSISTENT) {
                ds_put_format(ds, "%spersistent%s,",
                              colors.value, colors.end);
            }
            if (a->flags & NX_NAT_F_PROTO_HASH) {
                ds_put_format(ds, "%shash%s,", colors.value, colors.end);
            }
            if (a->flags & NX_NAT_F_PROTO_RANDOM) {
                ds_put_format(ds, "%srandom%s,", colors.value, colors.end);
            }
        }
        ds_chomp(ds, ',');
        ds_put_format(ds, "%s)%s", colors.paren, colors.end);
    }
}
