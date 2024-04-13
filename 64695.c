parse_value(const char *s, const char *delimiters)
{
    size_t n = 0;

    /* Iterate until we reach a delimiter.
     *
     * strchr(s, '\0') returns s+strlen(s), so this test handles the null
     * terminator at the end of 's'.  */
    while (!strchr(delimiters, s[n])) {
        if (s[n] == '(') {
            int level = 0;
            do {
                switch (s[n]) {
                case '\0':
                    return n;
                case '(':
                    level++;
                    break;
                case ')':
                    level--;
                    break;
                }
                n++;
            } while (level > 0);
        } else {
            n++;
        }
    }
    return n;
}
