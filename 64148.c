    OVS_EXCLUDED(ofproto_mutex)
{
    struct ofproto_group_mod ogm;

    ogm.gm.command = OFPGC11_DELETE;
    ogm.gm.group_id = OFPG_ALL;

    ovs_mutex_lock(&ofproto_mutex);
    ogm.version = ofproto->tables_version + 1;
    ofproto_group_mod_start(ofproto, &ogm);
    ofproto_bump_tables_version(ofproto);
    ofproto_group_mod_finish(ofproto, &ogm, NULL);
    ovs_mutex_unlock(&ofproto_mutex);
}
