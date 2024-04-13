ofputil_destroy_requestforward(struct ofputil_requestforward *rf)
{
    if (!rf) {
        return;
    }

    switch (rf->reason) {
    case OFPRFR_GROUP_MOD:
        ofputil_uninit_group_mod(rf->group_mod);
        free(rf->group_mod);
        break;

    case OFPRFR_METER_MOD:
        ofpbuf_uninit(&rf->bands);
        free(rf->meter_mod);
        break;

    case OFPRFR_N_REASONS:
        OVS_NOT_REACHED();
    }
}
