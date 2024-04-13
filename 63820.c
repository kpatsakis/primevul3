static float t1_scan_num(char *p, char **r)
{
    float f;
    skip_char(p, ' ');
    if (sscanf(p, "%g", &f) != 1) {
        remove_eol(p, t1_line_array);
        formatted_error("type 1","a number expected: '%s'", t1_line_array);
    }
    if (r != NULL) {
        for (; isdigit((unsigned char)*p) || *p == '.' ||
             *p == 'e' || *p == 'E' || *p == '+' || *p == '-'; p++);
        *r = p;
    }
    return f;
}
