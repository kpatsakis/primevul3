ofputil_versions_from_strings(char ** const s, size_t count)
{
    uint32_t bitmap = 0;

    while (count--) {
        int version = ofputil_version_from_string(s[count]);
        if (!version) {
            VLOG_WARN("Unknown OpenFlow version: \"%s\"", s[count]);
        } else {
            bitmap |= 1u << version;
        }
    }

    return bitmap;
}
