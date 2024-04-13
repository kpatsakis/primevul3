ofproto_mirror_register(struct ofproto *ofproto, void *aux,
                        const struct ofproto_mirror_settings *s)
{
    return (ofproto->ofproto_class->mirror_set
            ? ofproto->ofproto_class->mirror_set(ofproto, aux, s)
            : EOPNOTSUPP);
}
