ofproto_is_alive(const struct ofproto *p)
{
    return connmgr_has_controllers(p->connmgr);
}
