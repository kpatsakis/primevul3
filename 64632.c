ofputil_protocols_from_version_bitmap(uint32_t bitmap)
{
    enum ofputil_protocol protocols = 0;

    for (; bitmap; bitmap = zero_rightmost_1bit(bitmap)) {
        enum ofp_version version = rightmost_1bit_idx(bitmap);

        protocols |= ofputil_protocols_from_ofp_version(version);
    }

    return protocols;
}
