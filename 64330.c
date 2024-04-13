ofproto_mirror_get_stats(struct ofproto *ofproto, void *aux,
                         uint64_t *packets, uint64_t *bytes)
{
    if (!ofproto->ofproto_class->mirror_get_stats) {
        *packets = *bytes = UINT64_MAX;
        return EOPNOTSUPP;
    }

    return ofproto->ofproto_class->mirror_get_stats(ofproto, aux,
                                                    packets, bytes);
}
