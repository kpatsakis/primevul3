static void burl_offset_toupper (buffer * const b, const size_t off)
{
    /*(skips over all percent-encodings, including encoding of alpha chars)*/
    for (char *p = b->ptr+off; p[0]; ++p) {
        if (p[0] >= 'a' && p[0] <= 'z') p[0] &= 0xdf;
        else if (p[0]=='%' && light_isxdigit(p[1]) && light_isxdigit(p[2]))
            p+=2;
    }
}
