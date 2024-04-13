ofpact_is_set_or_move_action(const struct ofpact *a)
{
    switch (a->type) {
    case OFPACT_SET_FIELD:
    case OFPACT_REG_MOVE:
    case OFPACT_SET_ETH_DST:
    case OFPACT_SET_ETH_SRC:
    case OFPACT_SET_IP_DSCP:
    case OFPACT_SET_IP_ECN:
    case OFPACT_SET_IP_TTL:
    case OFPACT_SET_IPV4_DST:
    case OFPACT_SET_IPV4_SRC:
    case OFPACT_SET_L4_DST_PORT:
    case OFPACT_SET_L4_SRC_PORT:
    case OFPACT_SET_MPLS_LABEL:
    case OFPACT_SET_MPLS_TC:
    case OFPACT_SET_MPLS_TTL:
    case OFPACT_SET_QUEUE:
    case OFPACT_SET_TUNNEL:
    case OFPACT_SET_VLAN_PCP:
    case OFPACT_SET_VLAN_VID:
        return true;
    case OFPACT_BUNDLE:
    case OFPACT_CLEAR_ACTIONS:
    case OFPACT_CT:
    case OFPACT_CT_CLEAR:
    case OFPACT_CLONE:
    case OFPACT_NAT:
    case OFPACT_CONTROLLER:
    case OFPACT_DEC_MPLS_TTL:
    case OFPACT_DEC_TTL:
    case OFPACT_ENQUEUE:
    case OFPACT_EXIT:
    case OFPACT_UNROLL_XLATE:
    case OFPACT_FIN_TIMEOUT:
    case OFPACT_GOTO_TABLE:
    case OFPACT_GROUP:
    case OFPACT_LEARN:
    case OFPACT_CONJUNCTION:
    case OFPACT_METER:
    case OFPACT_MULTIPATH:
    case OFPACT_NOTE:
    case OFPACT_OUTPUT:
    case OFPACT_OUTPUT_REG:
    case OFPACT_OUTPUT_TRUNC:
    case OFPACT_POP_MPLS:
    case OFPACT_POP_QUEUE:
    case OFPACT_PUSH_MPLS:
    case OFPACT_PUSH_VLAN:
    case OFPACT_RESUBMIT:
    case OFPACT_SAMPLE:
    case OFPACT_STACK_POP:
    case OFPACT_STACK_PUSH:
    case OFPACT_STRIP_VLAN:
    case OFPACT_WRITE_ACTIONS:
    case OFPACT_WRITE_METADATA:
    case OFPACT_DEBUG_RECIRC:
        return false;
    default:
        OVS_NOT_REACHED();
    }
}
