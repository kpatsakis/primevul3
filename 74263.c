uri_decoded_copy (const char *part, 
                  gsize length)
{
    unsigned char *s, *d;
    char *decoded = g_strndup (part, length);

    s = d = (unsigned char *)decoded;
    do {
        if (*s == '%') {
            if (!g_ascii_isxdigit (s[1]) ||
                !g_ascii_isxdigit (s[2])) {
                *d++ = *s;
                continue;
            }
            *d++ = HEXCHAR (s);
            s += 2;
        } else {
            *d++ = *s;
        }
    } while (*s++);

    return decoded;
}
