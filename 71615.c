static void burl_append_encode_psnde (buffer * const b, const char * const str, const size_t len)
{
    /* percent-encodes everything except unreserved  - . 0-9 A-Z _ a-z ~ plus /
     * unless already percent-encoded (does not double-encode) */
    /* Note: not checking for invalid UTF-8 */
    char * const p = buffer_string_prepare_append(b, len*3);
    unsigned int n1, n2;
    int j = 0;
    for (unsigned int i = 0; i < len; ++i, ++j) {
        if (str[i]=='%' && li_cton(str[i+1], n1) && li_cton(str[i+2], n2)) {
            const unsigned int x = (n1 << 4) | n2;
            if (burl_is_unreserved((int)x)) {
                p[j] = (char)x;
            }
            else { /* leave UTF-8, control chars, and required chars encoded */
                p[j]   = '%';
                p[++j] = str[i+1];
                p[++j] = str[i+2];
            }
            i+=2;
        }
        else if (burl_is_unreserved(str[i]) || str[i] == '/') {
            p[j] = str[i];
        }
        else {
            p[j]   = '%';
            p[++j] = hex_chars_uc[(str[i] >> 4) & 0xF];
            p[++j] = hex_chars_uc[str[i] & 0xF];
        }
    }
    buffer_commit(b, j);
}
