static int isokflag(char *s, int *isseen)
{
    if (s[0] == '\\') {
        lcase(s);
        if (!strcmp(s, "\\seen")) {
            *isseen = 1;
            return 1;
        }
        if (!strcmp(s, "\\answered")) return 1;
        if (!strcmp(s, "\\flagged")) return 1;
        if (!strcmp(s, "\\draft")) return 1;
        if (!strcmp(s, "\\deleted")) return 1;

        /* uh oh, system flag i don't recognize */
        return 0;
    } else {
        /* valid user flag? */
        return imparse_isatom(s);
    }
}
