ofproto_reconnect_controllers(struct ofproto *ofproto)
{
    connmgr_reconnect(ofproto->connmgr);
}
