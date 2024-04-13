ofputil_async_cfg_default(enum ofp_version version)
{
    /* We enable all of the OF1.4 reasons regardless of 'version' because the
     * reasons added in OF1.4 just are just refinements of the OFPR_ACTION
     * introduced in OF1.0, breaking it into more specific categories.  When we
     * encode these for earlier OpenFlow versions, we translate them into
     * OFPR_ACTION.  */
    uint32_t pin = OFPR14_BITS & ~(1u << OFPR_INVALID_TTL);
    pin |= 1u << OFPR_EXPLICIT_MISS;
    if (version <= OFP12_VERSION) {
        pin |= 1u << OFPR_IMPLICIT_MISS;
    }

    return (struct ofputil_async_cfg) {
        .master[OAM_PACKET_IN] = pin,

        .master[OAM_FLOW_REMOVED]
            = (version >= OFP14_VERSION ? OFPRR14_BITS : OFPRR10_BITS),

        .master[OAM_PORT_STATUS] = OFPPR_BITS,
        .slave[OAM_PORT_STATUS] = OFPPR_BITS,
    };
}
