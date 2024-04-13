ofputil_protocol_to_base(enum ofputil_protocol protocol)
{
    return ofputil_protocol_set_tid(protocol, false);
}
