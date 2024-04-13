ofproto_mirror_unregister(struct ofproto *ofproto, void *aux)
{
    return ofproto_mirror_register(ofproto, aux, NULL);
}
