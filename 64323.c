ofproto_has_snoops(const struct ofproto *ofproto)
{
    return connmgr_has_snoops(ofproto->connmgr);
}
