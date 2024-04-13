ofputil_dl_type_from_openflow(ovs_be16 ofp_dl_type)
{
    return (ofp_dl_type == htons(OFP_DL_TYPE_NOT_ETH_TYPE)
            ? htons(FLOW_DL_TYPE_NONE)
            : ofp_dl_type);
}
