ofputil_encode_group_mod(enum ofp_version ofp_version,
                         const struct ofputil_group_mod *gm)
{

    switch (ofp_version) {
    case OFP10_VERSION:
        bad_group_cmd(gm->command);

    case OFP11_VERSION:
    case OFP12_VERSION:
    case OFP13_VERSION:
    case OFP14_VERSION:
        if (gm->command > OFPGC11_DELETE && gm->command != OFPGC11_ADD_OR_MOD) {
            bad_group_cmd(gm->command);
        }
        return ofputil_encode_ofp11_group_mod(ofp_version, gm);

    case OFP15_VERSION:
    case OFP16_VERSION:
        return ofputil_encode_ofp15_group_mod(ofp_version, gm);

    default:
        OVS_NOT_REACHED();
    }
}
