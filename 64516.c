ofputil_decode_table_eviction(ovs_be32 config, enum ofp_version version)
{
    return (version < OFP14_VERSION ? OFPUTIL_TABLE_EVICTION_DEFAULT
            : config & htonl(OFPTC14_EVICTION) ? OFPUTIL_TABLE_EVICTION_ON
            : OFPUTIL_TABLE_EVICTION_OFF);
}
