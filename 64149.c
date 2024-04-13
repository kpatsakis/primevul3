    OVS_EXCLUDED(ofproto_mutex)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct ofproto_group_mod ogm;
    enum ofperr error;

    error = reject_slave_controller(ofconn);
    if (error) {
        return error;
    }

    error = ofputil_decode_group_mod(oh, &ogm.gm);
    if (error) {
        return error;
    }

    ovs_mutex_lock(&ofproto_mutex);
    ogm.version = ofproto->tables_version + 1;
    error = ofproto_group_mod_start(ofproto, &ogm);
    if (!error) {
        struct openflow_mod_requester req = { ofconn, oh };

        ofproto_bump_tables_version(ofproto);
        ofproto_group_mod_finish(ofproto, &ogm, &req);
        ofmonitor_flush(ofproto->connmgr);
    }
    ovs_mutex_unlock(&ofproto_mutex);

    ofputil_uninit_group_mod(&ogm.gm);

    return error;
}
