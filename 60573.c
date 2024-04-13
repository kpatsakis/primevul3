static void get_word(char *buf, int buf_size, const char **pp)
{
    const char *p;
    char *q;

#define SPACE_CHARS " \t\r\n"

    p = *pp;
    p += strspn(p, SPACE_CHARS);
    q = buf;
    while (!av_isspace(*p) && *p != '\0') {
        if ((q - buf) < buf_size - 1)
            *q++ = *p;
        p++;
    }
    if (buf_size > 0)
        *q = '\0';
    *pp = p;
}
