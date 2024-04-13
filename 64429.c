mf_bitmap_from_of10(ovs_be32 wc10_)
{
    struct mf_bitmap fields = MF_BITMAP_INITIALIZER;
    const struct ofp10_wc_map *p;
    uint32_t wc10 = ntohl(wc10_);

    for (p = ofp10_wc_map; p < &ofp10_wc_map[ARRAY_SIZE(ofp10_wc_map)]; p++) {
        if (wc10 & p->wc10) {
            bitmap_set1(fields.bm, p->mf);
        }
    }
    return fields;
}
