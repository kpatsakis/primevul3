mf_bitmap_from_of11(ovs_be32 wc11_)
{
    struct mf_bitmap fields = MF_BITMAP_INITIALIZER;
    const struct ofp11_wc_map *p;
    uint32_t wc11 = ntohl(wc11_);

    for (p = ofp11_wc_map; p < &ofp11_wc_map[ARRAY_SIZE(ofp11_wc_map)]; p++) {
        if (wc11 & p->wc11) {
            bitmap_set1(fields.bm, p->mf);
        }
    }
    return fields;
}
