ofputil_encode_table_config(enum ofputil_table_miss miss,
                            enum ofputil_table_eviction eviction,
                            enum ofputil_table_vacancy vacancy,
                            enum ofp_version version)
{
    uint32_t config = 0;
    /* Search for "OFPTC_* Table Configuration" in the documentation for more
     * information on the crazy evolution of this field. */
    switch (version) {
    case OFP10_VERSION:
        /* OpenFlow 1.0 didn't have such a field, any value ought to do. */
        return htonl(0);

    case OFP11_VERSION:
    case OFP12_VERSION:
        /* OpenFlow 1.1 and 1.2 define only OFPTC11_TABLE_MISS_*. */
        switch (miss) {
        case OFPUTIL_TABLE_MISS_DEFAULT:
            /* Really this shouldn't be used for encoding (the caller should
             * provide a specific value) but I can't imagine that defaulting to
             * the fall-through case here will hurt. */
        case OFPUTIL_TABLE_MISS_CONTROLLER:
        default:
            return htonl(OFPTC11_TABLE_MISS_CONTROLLER);
        case OFPUTIL_TABLE_MISS_CONTINUE:
            return htonl(OFPTC11_TABLE_MISS_CONTINUE);
        case OFPUTIL_TABLE_MISS_DROP:
            return htonl(OFPTC11_TABLE_MISS_DROP);
        }
        OVS_NOT_REACHED();

    case OFP13_VERSION:
        /* OpenFlow 1.3 removed OFPTC11_TABLE_MISS_* and didn't define any new
         * flags, so this is correct. */
        return htonl(0);

    case OFP14_VERSION:
    case OFP15_VERSION:
    case OFP16_VERSION:
        /* OpenFlow 1.4 introduced OFPTC14_EVICTION and
         * OFPTC14_VACANCY_EVENTS. */
        if (eviction == OFPUTIL_TABLE_EVICTION_ON) {
            config |= OFPTC14_EVICTION;
        }
        if (vacancy == OFPUTIL_TABLE_VACANCY_ON) {
            config |= OFPTC14_VACANCY_EVENTS;
        }
        return htonl(config);
    }

    OVS_NOT_REACHED();
}
