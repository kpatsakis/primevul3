mf_bitmap_to_of11(const struct mf_bitmap *fields)
{
    const struct ofp11_wc_map *p;
    uint32_t wc11 = 0;

    for (p = ofp11_wc_map; p < &ofp11_wc_map[ARRAY_SIZE(ofp11_wc_map)]; p++) {
        if (bitmap_is_set(fields->bm, p->mf)) {
            wc11 |= p->wc11;
        }
    }
    return htonl(wc11);
}
