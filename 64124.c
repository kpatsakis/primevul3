    OVS_EXCLUDED(ofproto_mutex)
{
    const struct ofproto_class *class;
    struct ofproto *ofproto;
    int error;
    int i;

    *ofprotop = NULL;

    datapath_type = ofproto_normalize_type(datapath_type);
    class = ofproto_class_find__(datapath_type);
    if (!class) {
        VLOG_WARN("could not create datapath %s of unknown type %s",
                  datapath_name, datapath_type);
        return EAFNOSUPPORT;
    }

    ofproto = class->alloc();
    if (!ofproto) {
        VLOG_ERR("failed to allocate datapath %s of type %s",
                 datapath_name, datapath_type);
        return ENOMEM;
    }

    /* Initialize. */
    ovs_mutex_lock(&ofproto_mutex);
    memset(ofproto, 0, sizeof *ofproto);
    ofproto->ofproto_class = class;
    ofproto->name = xstrdup(datapath_name);
    ofproto->type = xstrdup(datapath_type);
    hmap_insert(&all_ofprotos, &ofproto->hmap_node,
                hash_string(ofproto->name, 0));
    ofproto->datapath_id = 0;
    ofproto->forward_bpdu = false;
    ofproto->fallback_dpid = pick_fallback_dpid();
    ofproto->mfr_desc = NULL;
    ofproto->hw_desc = NULL;
    ofproto->sw_desc = NULL;
    ofproto->serial_desc = NULL;
    ofproto->dp_desc = NULL;
    ofproto->frag_handling = OFPUTIL_FRAG_NORMAL;
    hmap_init(&ofproto->ports);
    hmap_init(&ofproto->ofport_usage);
    shash_init(&ofproto->port_by_name);
    simap_init(&ofproto->ofp_requests);
    ofproto->max_ports = ofp_to_u16(OFPP_MAX);
    ofproto->eviction_group_timer = LLONG_MIN;
    ofproto->tables = NULL;
    ofproto->n_tables = 0;
    ofproto->tables_version = OVS_VERSION_MIN;
    hindex_init(&ofproto->cookies);
    hmap_init(&ofproto->learned_cookies);
    ovs_list_init(&ofproto->expirable);
    ofproto->connmgr = connmgr_create(ofproto, datapath_name, datapath_name);
    ofproto->min_mtu = INT_MAX;
    cmap_init(&ofproto->groups);
    ovs_mutex_unlock(&ofproto_mutex);
    ofproto->ogf.types = 0xf;
    ofproto->ogf.capabilities = OFPGFC_CHAINING | OFPGFC_SELECT_LIVENESS |
                                OFPGFC_SELECT_WEIGHT;
    for (i = 0; i < 4; i++) {
        ofproto->ogf.max_groups[i] = OFPG_MAX;
        ofproto->ogf.ofpacts[i] = (UINT64_C(1) << N_OFPACTS) - 1;
    }
    ovsrcu_set(&ofproto->metadata_tab, tun_metadata_alloc(NULL));

    ovs_mutex_init(&ofproto->vl_mff_map.mutex);
    cmap_init(&ofproto->vl_mff_map.cmap);

    error = ofproto->ofproto_class->construct(ofproto);
    if (error) {
        VLOG_ERR("failed to open datapath %s: %s",
                 datapath_name, ovs_strerror(error));
        ovs_mutex_lock(&ofproto_mutex);
        connmgr_destroy(ofproto->connmgr);
        ofproto->connmgr = NULL;
        ovs_mutex_unlock(&ofproto_mutex);
        ofproto_destroy__(ofproto);
        return error;
    }

    /* Check that hidden tables, if any, are at the end. */
    ovs_assert(ofproto->n_tables);
    for (i = 0; i + 1 < ofproto->n_tables; i++) {
        enum oftable_flags flags = ofproto->tables[i].flags;
        enum oftable_flags next_flags = ofproto->tables[i + 1].flags;

        ovs_assert(!(flags & OFTABLE_HIDDEN) || next_flags & OFTABLE_HIDDEN);
    }

    ofproto->datapath_id = pick_datapath_id(ofproto);
    init_ports(ofproto);

    /* Initialize meters table. */
    if (ofproto->ofproto_class->meter_get_features) {
        ofproto->ofproto_class->meter_get_features(ofproto,
                                                   &ofproto->meter_features);
    } else {
        memset(&ofproto->meter_features, 0, sizeof ofproto->meter_features);
    }
    ofproto->meters = xzalloc((ofproto->meter_features.max_meters + 1)
                              * sizeof(struct meter *));

    /* Set the initial tables version. */
    ofproto_bump_tables_version(ofproto);

    *ofprotop = ofproto;
    return 0;
}
