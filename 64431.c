mf_bitmap_to_of10(const struct mf_bitmap *fields)
{
    const struct ofp10_wc_map *p;
    uint32_t wc10 = 0;

    for (p = ofp10_wc_map; p < &ofp10_wc_map[ARRAY_SIZE(ofp10_wc_map)]; p++) {
        if (bitmap_is_set(fields->bm, p->mf)) {
            wc10 |= p->wc10;
        }
    }
    return htonl(wc10);
}
