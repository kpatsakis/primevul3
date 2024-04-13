ofproto_is_mirror_output_bundle(const struct ofproto *ofproto, void *aux)
{
    return (ofproto->ofproto_class->is_mirror_output_bundle
            ? ofproto->ofproto_class->is_mirror_output_bundle(ofproto, aux)
            : false);
}
