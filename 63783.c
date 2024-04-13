static float t1_scan_num(char *p, char **r)
{
    float f;
    skip(p, ' ');
    if (sscanf(p, "%g", &f) != 1) {
        remove_eol(p, t1_line_array);
        pdftex_fail("a number expected: `%s'", t1_line_array);
    }
    if (r != NULL) {
        for (; isdigit((unsigned char)*p) || *p == '.' ||
             *p == 'e' || *p == 'E' || *p == '+' || *p == '-'; p++);
        *r = p;
    }
    return f;
}
