format_WRITE_METADATA(const struct ofpact_metadata *a, struct ds *s)
{
    ds_put_format(s, "%swrite_metadata:%s%#"PRIx64,
                  colors.param, colors.end, ntohll(a->metadata));
    if (a->mask != OVS_BE64_MAX) {
        ds_put_format(s, "/%#"PRIx64, ntohll(a->mask));
    }
}
