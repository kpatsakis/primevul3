ofport_remove(struct ofport *ofport)
{
    struct ofproto *p = ofport->ofproto;
    bool is_mtu_overridden = ofport_is_mtu_overridden(p, ofport);

    connmgr_send_port_status(ofport->ofproto->connmgr, NULL, &ofport->pp,
                             OFPPR_DELETE);
    ofport_destroy(ofport, true);
    if (!is_mtu_overridden) {
        update_mtu_ofproto(p);
    }
}
