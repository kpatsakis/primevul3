ofproto_bundle_unregister(struct ofproto *ofproto, void *aux)
{
    return ofproto_bundle_register(ofproto, aux, NULL);
}
