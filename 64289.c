ofproto_aa_mapping_register(struct ofproto *ofproto, void *aux,
                            const struct aa_mapping_settings *s)
{
    if (!ofproto->ofproto_class->aa_mapping_set) {
        return EOPNOTSUPP;
    }
    ofproto->ofproto_class->aa_mapping_set(ofproto, aux, s);
    return 0;
}
