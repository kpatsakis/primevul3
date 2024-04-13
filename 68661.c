static int wm_strcasecmp(const char *s1, const char * s2) {
    const char * p1 = s1;
    const char * p2 = s2;
    char c1, c2;

    if (p1 == p2) return 0;
    do {
        c1 = wm_tolower (*p1++);
        c2 = wm_tolower (*p2++);
        if (c1 == '\0') break;
    } while (c1 == c2);
    return (int)(c1 - c2);
}
