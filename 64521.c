ofputil_decode_table_vacancy(ovs_be32 config, enum ofp_version version)
{
    return (version < OFP14_VERSION ? OFPUTIL_TABLE_VACANCY_DEFAULT
            : config & htonl(OFPTC14_VACANCY_EVENTS) ? OFPUTIL_TABLE_VACANCY_ON
            : OFPUTIL_TABLE_VACANCY_OFF);
}
