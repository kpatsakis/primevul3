ofproto_set_datapath_id(struct ofproto *p, uint64_t datapath_id)
{
    uint64_t old_dpid = p->datapath_id;
    p->datapath_id = datapath_id ? datapath_id : pick_datapath_id(p);
    if (p->datapath_id != old_dpid) {
        /* Force all active connections to reconnect, since there is no way to
         * notify a controller that the datapath ID has changed. */
        ofproto_reconnect_controllers(p);
    }
}
