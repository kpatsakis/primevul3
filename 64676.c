ofputil_version_from_string(const char *s)
{
    if (!strcasecmp(s, "OpenFlow10")) {
        return OFP10_VERSION;
    }
    if (!strcasecmp(s, "OpenFlow11")) {
        return OFP11_VERSION;
    }
    if (!strcasecmp(s, "OpenFlow12")) {
        return OFP12_VERSION;
    }
    if (!strcasecmp(s, "OpenFlow13")) {
        return OFP13_VERSION;
    }
    if (!strcasecmp(s, "OpenFlow14")) {
        return OFP14_VERSION;
    }
    if (!strcasecmp(s, "OpenFlow15")) {
        return OFP15_VERSION;
    }
    if (!strcasecmp(s, "OpenFlow16")) {
        return OFP16_VERSION;
    }
    return 0;
}
