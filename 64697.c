put_fields_property(struct ofpbuf *reply,
                    const struct mf_bitmap *fields,
                    const struct mf_bitmap *masks,
                    enum ofp13_table_feature_prop_type property,
                    enum ofp_version version)
{
    size_t start_ofs;
    int field;

    start_ofs = ofpprop_start(reply, property);
    BITMAP_FOR_EACH_1 (field, MFF_N_IDS, fields->bm) {
        nx_put_header(reply, field, version,
                      masks && bitmap_is_set(masks->bm, field));
    }
    ofpprop_end(reply, start_ofs);
}
