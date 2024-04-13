static void t1_scan_keys(PDF pdf)
{
    int i, k;
    char *p, *q, *r;
    const key_entry *key;
    if (t1_prefix("/FontType")) {
        p = t1_line_array + strlen("FontType") + 1;
        if ((i = (int) t1_scan_num(p, 0)) != 1)
            formatted_error("type 1","Type%d fonts unsupported by backend", i);
        return;
    }
    for (key = (const key_entry *) font_key; key - font_key < FONT_KEYS_NUM;
         key++) {
        if (key->t1name[0] != '\0'
            && str_prefix(t1_line_array + 1, key->t1name))
            break;
    }
    if (key - font_key == FONT_KEYS_NUM)
        return;
    p = t1_line_array + strlen(key->t1name) + 1;
    skip_char(p, ' ');
    if ((k = (int) (key - font_key)) == FONTNAME_CODE) {
        if (*p != '/') {
            remove_eol(p, t1_line_array);
            formatted_error("type 1","a name expected: '%s'", t1_line_array);
        }
        /*tex Skip the slash. */
        r = ++p;
        for (q = t1_buf_array; *p != ' ' && *p != 10; *q++ = *p++);
        *q = 0;
        xfree(fd_cur->fontname);
        fd_cur->fontname = xstrdup(t1_buf_array);
        /*tex

            At this moment we cannot call |make_subset_tag| yet, as the encoding
            is not read; thus we mark the offset of the subset tag and write it
            later.

        */
        if (is_subsetted(fd_cur->fm)) {
            t1_fontname_offset = (int) (t1_offset() + (r - t1_line_array));
            strcpy(t1_buf_array, p);
            sprintf(r, "ABCDEF+%s%s", fd_cur->fontname, t1_buf_array);
            t1_line_ptr = eol(r);
        }
        return;
    }
    if ((k == STEMV_CODE || k == FONTBBOX1_CODE) && (*p == '[' || *p == '{'))
        p++;
    if (k == FONTBBOX1_CODE) {
        for (i = 0; i < 4; i++, k++) {
            fd_cur->font_dim[k].val = (int) t1_scan_num(p, &r);
            fd_cur->font_dim[k].set = true;
            p = r;
        }
        return;
    }
    fd_cur->font_dim[k].val = (int) t1_scan_num(p, 0);
    fd_cur->font_dim[k].set = true;
}

static void t1_scan_param(PDF pdf)
{
    static const char *lenIV = "/lenIV";
    if (!t1_scan || *t1_line_array != '/')
        return;
    if (t1_prefix(lenIV)) {
        t1_lenIV = (short) t1_scan_num(t1_line_array + strlen(lenIV), 0);
        if (t1_lenIV < 0)
            normal_error("type 1","negative value of lenIV is not supported");
        return;
    }
    t1_scan_keys(pdf);
}

static void copy_glyph_names(char **glyph_names, int a, int b)
{
    if (glyph_names[b] != notdef) {
        xfree(glyph_names[b]);
        glyph_names[b] = (char *) notdef;
    }
    if (glyph_names[a] != notdef) {
        glyph_names[b] = xstrdup(glyph_names[a]);
    }
}

/*tex Read encoding from Type1 font file, return |glyph_names| array, or |pdffail|. */

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
        \stoptyping

    */
    t1_encoding = ENC_BUILTIN;
    if (t1_prefix("/Encoding [") || t1_prefix("/Encoding[")) {  /* the first case */
        r = strchr(t1_line_array, '[') + 1;
        skip_char(r, ' ');
        for (;;) {
            while (*r == '/') {
                for (p = t1_buf_array, r++; *r != 32 && *r != 10 && *r != ']' && *r != '/'; *p++ = *r++);
                *p = 0;
                skip_char(r, ' ');
                if (counter > 255)
                    normal_error("type 1","encoding vector contains more than 256 names");
                if (strcmp(t1_buf_array, notdef) != 0)
                    glyph_names[counter] = xstrdup(t1_buf_array);
                counter++;
            }
            if (*r != 10 && *r != '%') {
                if (str_prefix(r, "] def") || str_prefix(r, "] readonly def"))
                    break;
                else {
                    remove_eol(r, t1_line_array);
                    formatted_error("type 1","a name or '] def' or '] readonly def' expected: '%s'", t1_line_array);
                }
            }
            t1_getline();
            r = t1_line_array;
        }
    } else {
        /*tex The second case. */
        p = strchr(t1_line_array, 10);
        for (;;) {
            if (*p == 10) {
                t1_getline();
                p = t1_line_array;
            }
            /*tex Check for |dup <index> <glyph> put|. */
            if (sscanf(p, "dup %i%255s put", &i, t1_buf_array) == 2 &&
                *t1_buf_array == '/' && valid_code(i)) {
                if (strcmp(t1_buf_array + 1, notdef) != 0)
                    glyph_names[i] = xstrdup(t1_buf_array + 1);
                p = strstr(p, " put") + strlen(" put");
                skip_char(p, ' ');
            }
            /*tex Check for |dup dup <to> exch <from> get put|. */
            else if (sscanf(p, "dup dup %i exch %i get put", &b, &a) == 2 && valid_code(a) && valid_code(b)) {
                copy_glyph_names(glyph_names, a, b);
                p = strstr(p, " get put") + strlen(" get put");
                skip_char(p, ' ');
            }
            /*tex Check for |dup dup <from> <size> getinterval <to> exch putinterval|. */
            else if (sscanf(p, "dup dup %i %i getinterval %i exch putinterval",
                    &a, &c, &b) == 3 && valid_code(a) && valid_code(b) && valid_code(c)) {
                for (i = 0; i < c; i++)
                    copy_glyph_names(glyph_names, a + i, b + i);
                p = strstr(p, " putinterval") + strlen(" putinterval");
                skip_char(p, ' ');
            }
            /*tex Check for |def or |readonly def|. */
            else if ((p == t1_line_array || (p > t1_line_array && p[-1] == ' ')) && strcmp(p, "def\n") == 0) {
                return glyph_names;
            } else {
                /*tex Skip an unrecognizable word. */
                while (*p != ' ' && *p != 10)
                    p++;
                skip_char(p, ' ');
            }
        }
    }
    return glyph_names;
}
