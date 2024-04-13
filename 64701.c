put_table_instruction_features(
    struct ofpbuf *reply, const struct ofputil_table_instruction_features *tif,
    int miss_offset, enum ofp_version version)
{
    size_t start_ofs;
    uint8_t table_id;

    ofpprop_put_bitmap(reply, OFPTFPT13_INSTRUCTIONS + miss_offset,
                       ntohl(ovsinst_bitmap_to_openflow(tif->instructions,
                                                        version)));

    start_ofs = ofpprop_start(reply, OFPTFPT13_NEXT_TABLES + miss_offset);
    BITMAP_FOR_EACH_1 (table_id, 255, tif->next) {
        ofpbuf_put(reply, &table_id, 1);
    }
    ofpprop_end(reply, start_ofs);

    put_table_action_features(reply, &tif->write,
                              OFPTFPT13_WRITE_ACTIONS,
                              OFPTFPT13_WRITE_SETFIELD, miss_offset, version);
    put_table_action_features(reply, &tif->apply,
                              OFPTFPT13_APPLY_ACTIONS,
                              OFPTFPT13_APPLY_SETFIELD, miss_offset, version);
}
