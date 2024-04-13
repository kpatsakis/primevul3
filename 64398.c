query_switch_features(struct ofproto *ofproto,
                      bool *arp_match_ip, uint64_t *ofpacts)
{
    struct ofputil_table_features *features, *f;

    *arp_match_ip = false;
    *ofpacts = 0;

    query_tables(ofproto, &features, NULL);
    for (f = features; f < &features[ofproto->n_tables]; f++) {
        *ofpacts |= f->nonmiss.apply.ofpacts | f->miss.apply.ofpacts;
        if (bitmap_is_set(f->match.bm, MFF_ARP_SPA) ||
            bitmap_is_set(f->match.bm, MFF_ARP_TPA)) {
            *arp_match_ip = true;
        }
    }
    free(features);

    /* Sanity check. */
    ovs_assert(*ofpacts & (UINT64_C(1) << OFPACT_OUTPUT));
}
