ofproto_set_aa(struct ofproto *ofproto, void *aux OVS_UNUSED,
               const struct aa_settings *s)
{
    if (!ofproto->ofproto_class->set_aa) {
        return EOPNOTSUPP;
    }
    ofproto->ofproto_class->set_aa(ofproto, s);
    return 0;
}
