ofputil_decode_hello(const struct ofp_header *oh, uint32_t *allowed_versions)
{
    struct ofpbuf msg = ofpbuf_const_initializer(oh, ntohs(oh->length));
    ofpbuf_pull(&msg, sizeof *oh);

    *allowed_versions = version_bitmap_from_version(oh->version);

    bool ok = true;
    while (msg.size) {
        const struct ofp_hello_elem_header *oheh;
        unsigned int len;

        if (msg.size < sizeof *oheh) {
            return false;
        }

        oheh = msg.data;
        len = ntohs(oheh->length);
        if (len < sizeof *oheh || !ofpbuf_try_pull(&msg, ROUND_UP(len, 8))) {
            return false;
        }

        if (oheh->type != htons(OFPHET_VERSIONBITMAP)
            || !ofputil_decode_hello_bitmap(oheh, allowed_versions)) {
            ok = false;
        }
    }

    return ok;
}
