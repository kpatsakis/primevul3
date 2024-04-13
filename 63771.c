static boolean str_suffix(const char *begin_buf, const char *end_buf,
                          const char *s)
{
    const char *s1 = end_buf - 1, *s2 = strend(s) - 1;
    if (*s1 == 10)
        s1--;
    while (s1 >= begin_buf && s2 >= s) {
        if (*s1-- != *s2--)
            return false;
    }
    return s2 < s;
}
