ofputil_version_to_string(enum ofp_version ofp_version)
{
    switch (ofp_version) {
    case OFP10_VERSION:
        return "OpenFlow10";
    case OFP11_VERSION:
        return "OpenFlow11";
    case OFP12_VERSION:
        return "OpenFlow12";
    case OFP13_VERSION:
        return "OpenFlow13";
    case OFP14_VERSION:
        return "OpenFlow14";
    case OFP15_VERSION:
        return "OpenFlow15";
    case OFP16_VERSION:
        return "OpenFlow16";
    default:
        OVS_NOT_REACHED();
    }
}
