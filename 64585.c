ofputil_free_bundle_msgs(struct ofputil_bundle_msg *bms, size_t n_bms)
{
    for (size_t i = 0; i < n_bms; i++) {
        switch ((int)bms[i].type) {
        case OFPTYPE_FLOW_MOD:
            free(CONST_CAST(struct ofpact *, bms[i].fm.ofpacts));
            break;
        case OFPTYPE_GROUP_MOD:
            ofputil_uninit_group_mod(&bms[i].gm);
            break;
        case OFPTYPE_PACKET_OUT:
            free(bms[i].po.ofpacts);
            free(CONST_CAST(void *, bms[i].po.packet));
            break;
        default:
            break;
        }
    }
    free(bms);
}
