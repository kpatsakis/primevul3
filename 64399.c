query_tables(struct ofproto *ofproto,
             struct ofputil_table_features **featuresp,
             struct ofputil_table_stats **statsp)
{
    struct mf_bitmap rw_fields = oxm_writable_fields();
    struct mf_bitmap match = oxm_matchable_fields();
    struct mf_bitmap mask = oxm_maskable_fields();

    struct ofputil_table_features *features;
    struct ofputil_table_stats *stats;
    int i;

    features = *featuresp = xcalloc(ofproto->n_tables, sizeof *features);
    for (i = 0; i < ofproto->n_tables; i++) {
        struct ofputil_table_features *f = &features[i];

        f->table_id = i;
        sprintf(f->name, "table%d", i);
        f->metadata_match = OVS_BE64_MAX;
        f->metadata_write = OVS_BE64_MAX;
        atomic_read_relaxed(&ofproto->tables[i].miss_config, &f->miss_config);
        f->max_entries = 1000000;

        bool more_tables = false;
        for (int j = i + 1; j < ofproto->n_tables; j++) {
            if (!(ofproto->tables[j].flags & OFTABLE_HIDDEN)) {
                bitmap_set1(f->nonmiss.next, j);
                more_tables = true;
            }
        }
        f->nonmiss.instructions = (1u << N_OVS_INSTRUCTIONS) - 1;
        if (!more_tables) {
            f->nonmiss.instructions &= ~(1u << OVSINST_OFPIT11_GOTO_TABLE);
        }
        f->nonmiss.write.ofpacts = (UINT64_C(1) << N_OFPACTS) - 1;
        f->nonmiss.write.set_fields = rw_fields;
        f->nonmiss.apply = f->nonmiss.write;
        f->miss = f->nonmiss;

        f->match = match;
        f->mask = mask;
        f->wildcard = match;
    }

    if (statsp) {
        stats = *statsp = xcalloc(ofproto->n_tables, sizeof *stats);
        for (i = 0; i < ofproto->n_tables; i++) {
            struct ofputil_table_stats *s = &stats[i];

            s->table_id = i;
            s->active_count = ofproto->tables[i].n_flows;
            if (i == 0) {
                s->active_count -= connmgr_count_hidden_rules(
                    ofproto->connmgr);
            }
        }
    } else {
        stats = NULL;
    }

    ofproto->ofproto_class->query_tables(ofproto, features, stats);

    for (i = 0; i < ofproto->n_tables; i++) {
        const struct oftable *table = &ofproto->tables[i];
        struct ofputil_table_features *f = &features[i];

        if (table->name) {
            ovs_strzcpy(f->name, table->name, sizeof f->name);
        }

        if (table->max_flows < f->max_entries) {
            f->max_entries = table->max_flows;
        }
    }
}
