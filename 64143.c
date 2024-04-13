    OVS_EXCLUDED(ofproto_mutex)
{
    struct ofproto_flow_mod ofm;
    enum ofperr error;

    error = ofproto_flow_mod_init(ofproto, &ofm, fm, NULL);
    if (error) {
        return error;
    }

    ovs_mutex_lock(&ofproto_mutex);
    ofm.version = ofproto->tables_version + 1;
    error = ofproto_flow_mod_start(ofproto, &ofm);
    if (!error) {
        ofproto_bump_tables_version(ofproto);
        ofproto_flow_mod_finish(ofproto, &ofm, req);
        ofmonitor_flush(ofproto->connmgr);
    }
    ovs_mutex_unlock(&ofproto_mutex);

    return error;
}
