handle_tlv_table_mod(struct ofconn *ofconn, const struct ofp_header *oh)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct tun_table *old_tab, *new_tab;
    struct ofputil_tlv_table_mod ttm;
    enum ofperr error;

    error = reject_slave_controller(ofconn);
    if (error) {
        return error;
    }

    error = ofputil_decode_tlv_table_mod(oh, &ttm);
    if (error) {
        return error;
    }

    old_tab = ovsrcu_get_protected(struct tun_table *, &ofproto->metadata_tab);
    error = tun_metadata_table_mod(&ttm, old_tab, &new_tab);
    if (!error) {
        ovs_mutex_lock(&ofproto->vl_mff_map.mutex);
        error = mf_vl_mff_map_mod_from_tun_metadata(&ofproto->vl_mff_map,
                                                    &ttm);
        ovs_mutex_unlock(&ofproto->vl_mff_map.mutex);
        if (!error) {
            ovsrcu_set(&ofproto->metadata_tab, new_tab);
            tun_metadata_postpone_free(old_tab);
        }
    }

    ofputil_uninit_tlv_table(&ttm.mappings);
    return error;
}
