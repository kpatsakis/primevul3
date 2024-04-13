ofputil_async_msg_type_to_string(enum ofputil_async_msg_type type)
{
    switch (type) {
    case OAM_PACKET_IN:      return "PACKET_IN";
    case OAM_PORT_STATUS:    return "PORT_STATUS";
    case OAM_FLOW_REMOVED:   return "FLOW_REMOVED";
    case OAM_ROLE_STATUS:    return "ROLE_STATUS";
    case OAM_TABLE_STATUS:   return "TABLE_STATUS";
    case OAM_REQUESTFORWARD: return "REQUESTFORWARD";

    case OAM_N_TYPES:
    default:
        OVS_NOT_REACHED();
    }
}
