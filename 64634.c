ofputil_protocols_to_version_bitmap(enum ofputil_protocol protocols)
{
    uint32_t bitmap = 0;

    for (; protocols; protocols = zero_rightmost_1bit(protocols)) {
        enum ofputil_protocol protocol = rightmost_1bit(protocols);

        bitmap |= 1u << ofputil_protocol_to_ofp_version(protocol);
    }

    return bitmap;
}
