    OVS_EXCLUDED(ofproto_mutex)
{
    struct oftable *table;

    cmap_destroy(&ofproto->groups);

    hmap_remove(&all_ofprotos, &ofproto->hmap_node);

    free(ofproto->name);
    free(ofproto->type);
    free(ofproto->mfr_desc);
    free(ofproto->hw_desc);
    free(ofproto->sw_desc);
    free(ofproto->serial_desc);
    free(ofproto->dp_desc);
    hmap_destroy(&ofproto->ports);
    hmap_destroy(&ofproto->ofport_usage);
    shash_destroy(&ofproto->port_by_name);
    simap_destroy(&ofproto->ofp_requests);

    OFPROTO_FOR_EACH_TABLE (table, ofproto) {
        oftable_destroy(table);
    }
    free(ofproto->tables);

    ovs_mutex_lock(&ofproto->vl_mff_map.mutex);
    mf_vl_mff_map_clear(&ofproto->vl_mff_map, true);
    ovs_mutex_unlock(&ofproto->vl_mff_map.mutex);
    cmap_destroy(&ofproto->vl_mff_map.cmap);
    ovs_mutex_destroy(&ofproto->vl_mff_map.mutex);
    tun_metadata_free(ovsrcu_get_protected(struct tun_table *,
                                           &ofproto->metadata_tab));

    ovs_assert(hindex_is_empty(&ofproto->cookies));
    hindex_destroy(&ofproto->cookies);

    ovs_assert(hmap_is_empty(&ofproto->learned_cookies));
    hmap_destroy(&ofproto->learned_cookies);

    ofproto->ofproto_class->dealloc(ofproto);
}
