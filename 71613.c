void burl_append (buffer * const b, const char * const str, const size_t len, const int flags)
{
    size_t off = 0;

    if (0 == len) return;

    if (0 == flags) {
        buffer_append_string_len(b, str, len);
        return;
    }

    if (flags & (BURL_TOUPPER|BURL_TOLOWER)) off = buffer_string_length(b);

    if (flags & BURL_ENCODE_NONE) {
        buffer_append_string_len(b, str, len);
    }
    else if (flags & BURL_ENCODE_ALL) {
        burl_append_encode_all(b, str, len);
    }
    else if (flags & BURL_ENCODE_NDE) {
        burl_append_encode_nde(b, str, len);
    }
    else if (flags & BURL_ENCODE_PSNDE) {
        burl_append_encode_psnde(b, str, len);
    }
    else if (flags & BURL_ENCODE_B64U) {
        const unsigned char *s = (const unsigned char *)str;
        buffer_append_base64_encode_no_padding(b, s, len, BASE64_URL);
    }
    else if (flags & BURL_DECODE_B64U) {
        buffer_append_base64_decode(b, str, len, BASE64_URL);
    }

    /* note: not normalizing str, which could come from arbitrary header,
     * so it is possible that alpha chars are percent-encoded upper/lowercase */
    if (flags & (BURL_TOLOWER|BURL_TOUPPER)) {
        (flags & BURL_TOLOWER)
          ? burl_offset_tolower(b, off)  /*(flags & BURL_TOLOWER)*/
          : burl_offset_toupper(b, off); /*(flags & BURL_TOUPPER)*/
    }
}
