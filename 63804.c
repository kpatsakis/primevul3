static char **t1_builtin_enc(void)
{
    int i, a, b, c, counter = 0;
    char *r, *p, **glyph_names;
    /*tex At this moment |/Encoding| is the prefix of |t1_line_array|. */
    glyph_names = xtalloc(256, char *);
    for (i = 0; i < 256; i++)
        glyph_names[i] = (char *) notdef;
    if (t1_suffix("def")) {
        /*tex A predefined encoding: */
        sscanf(t1_line_array + strlen("/Encoding"), "%255s", t1_buf_array);
        if (strcmp(t1_buf_array, "StandardEncoding") == 0) {
            t1_encoding = ENC_STANDARD;
            for (i = 0; i < 256; i++) {
                if (standard_glyph_names[i] != notdef)
                    glyph_names[i] = xstrdup(standard_glyph_names[i]);
            }
            return glyph_names;
        } else
            formatted_error("type 1","cannot subset font (unknown predefined encoding '%s')",t1_buf_array);
    }
    /*

        At this moment |/Encoding| is the prefix of |t1_line_array|, and the
        encoding is not a predefined encoding. We have two possible forms of
        vector. The first case is

        \starttyping
        /Encoding [
            /a /b /c ...
        ] readonly def
        \stoptyping

        and the second case can look like

        \starttyping
        /Encoding 256 array 0 1 255 {
            1 index exch /.notdef put} for
            dup 0 /x put
            dup 1 /y put
            ...
        } readonly def
