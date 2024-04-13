ofputil_protocol_from_ofp_version(enum ofp_version version)
{
    return rightmost_1bit(ofputil_protocols_from_ofp_version(version));
}
