    OVS_REQUIRES(ofproto_mutex)
{
    uint32_t mid;

    mid = ofpacts_get_meter(ofpacts, ofpacts_len);
    if (mid && get_provider_meter_id(ofproto, mid) == UINT32_MAX) {
        return OFPERR_OFPMMFC_INVALID_METER;
    }

    const struct ofpact_group *a;
    OFPACT_FOR_EACH_TYPE_FLATTENED (a, GROUP, ofpacts, ofpacts_len) {
        if (!ofproto_group_exists(ofproto, a->group_id)) {
            return OFPERR_OFPBAC_BAD_OUT_GROUP;
        }
    }

    return 0;
}
