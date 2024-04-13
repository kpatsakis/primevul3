char **load_enc_file(char *enc_name)
{
    char buf[ENC_BUF_SIZE], *p, *r;
    int i, names_count;
    char **glyph_names;
    set_cur_file_name(enc_name);
    glyph_names = (char **) mymalloc(256 * sizeof(char *));
    for (i = 0; i < 256; i++)
        glyph_names[i] = notdef;
    if (!enc_open()) {
        pdftex_warn("cannot open encoding file for reading");
        cur_file_name = NULL;
        return glyph_names;
    }
    t1_log("{");
    t1_log(cur_file_name = full_file_name());
    enc_getline();
    if (*enc_line != '/' || (r = strchr(enc_line, '[')) == NULL) {
        remove_eol(r, enc_line);
        pdftex_fail
           ("invalid encoding vector (a name or `[' missing): `%s'", enc_line);
    }
    names_count = 0;
    r++;                        /* skip '[' */
    skip(r, ' ');
    for (;;) {
        while (*r == '/') {
            for (p = buf, r++;
                 *r != ' ' && *r != 10 && *r != ']' && *r != '/'; *p++ = *r++);
            *p = 0;
            skip(r, ' ');
            if (names_count > 255)
                pdftex_fail("encoding vector contains more than 256 names");
            if (strcmp(buf, notdef) != 0)
                glyph_names[names_count] = xstrdup(buf);
            names_count++;
        }
        if (*r != 10 && *r != '%') {
            if (str_prefix(r, "] def"))
                goto done;
            else {
                remove_eol(r, enc_line);
                pdftex_fail
       ("invalid encoding vector: a name or `] def' expected: `%s'", enc_line);
            }
        }
        enc_getline();
        r = enc_line;
    }
  done:
    enc_close();
    t1_log("}");
    cur_file_name = NULL;
    return glyph_names;
}
