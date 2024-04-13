ofproto_set_snoops(struct ofproto *ofproto, const struct sset *snoops)
{
    return connmgr_set_snoops(ofproto->connmgr, snoops);
}
