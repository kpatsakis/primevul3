put_table_action_features(struct ofpbuf *reply,
                          const struct ofputil_table_action_features *taf,
                          enum ofp13_table_feature_prop_type actions_type,
                          enum ofp13_table_feature_prop_type set_fields_type,
                          int miss_offset, enum ofp_version version)
{
    ofpprop_put_bitmap(reply, actions_type + miss_offset,
                       ntohl(ofpact_bitmap_to_openflow(taf->ofpacts,
                                                       version)));
    put_fields_property(reply, &taf->set_fields, NULL,
                        set_fields_type + miss_offset, version);
}
