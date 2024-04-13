ofputil_protocol_is_valid(enum ofputil_protocol protocol)
{
    return protocol & OFPUTIL_P_ANY && is_pow2(protocol);
}
