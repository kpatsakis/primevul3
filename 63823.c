static void t1_subset_ascii_part(PDF pdf)
{
    int j, *p;
    char *glyph, **gg, **glyph_names;
    struct avl_table *gl_tree;
    struct avl_traverser t;
    void **aa;
    t1_getline();
    while (!t1_prefix("/Encoding")) {
        t1_scan_param(pdf);
        t1_putline(pdf);
        t1_getline();
    }
    glyph_names = t1_builtin_enc();
    fd_cur->builtin_glyph_names = glyph_names;
    if (is_subsetted(fd_cur->fm)) {
        if (fd_cur->tx_tree != NULL) {
            /*tex Take over collected non-reencoded characters from \TeX. */
            avl_t_init(&t, fd_cur->tx_tree);
            for (p = (int *) avl_t_first(&t, fd_cur->tx_tree); p != NULL;
                 p = (int *) avl_t_next(&t)) {
                if ((char *) avl_find(fd_cur->gl_tree, glyph_names[*p]) == NULL) {
                    glyph = xstrdup(glyph_names[*p]);
                    aa = avl_probe(fd_cur->gl_tree, glyph);
                    assert(aa != NULL);
                }
            }
        }
        make_subset_tag(fd_cur);
        strncpy((char *) pdf->fb->data + t1_fontname_offset, fd_cur->subset_tag,6);
    }
    /*tex Now really all glyphs needed from this font are in the |fd_cur->gl_tree|. */
    if (t1_encoding == ENC_STANDARD)
        t1_puts(pdf, "/Encoding StandardEncoding def\n");
    else {
        t1_puts(pdf,"/Encoding 256 array\n0 1 255 {1 index exch /.notdef put} for\n");
        gl_tree = create_t1_glyph_tree(glyph_names);
        avl_t_init(&t, fd_cur->gl_tree);
        j = 0;
        for (glyph = (char *) avl_t_first(&t, fd_cur->gl_tree); glyph != NULL;
             glyph = (char *) avl_t_next(&t)) {
            if ((gg = (char **) avl_find(gl_tree, &glyph)) != NULL) {
                t1_printf(pdf, "dup %i /%s put\n", (int) (gg - glyph_names),*gg);
                j++;
            }
        }
        destroy_t1_glyph_tree(gl_tree);
        if (j == 0) {
            /*tex
                We didn't mark anything for the Encoding array. We add |{dup 0
                /.notdef put}| for compatibility with Acrobat 5.0.
            */
            t1_puts(pdf, "dup 0 /.notdef put\n");
        }
        t1_puts(pdf, "readonly def\n");
    }
    do {
        t1_getline();
        t1_scan_param(pdf);
        if (!t1_prefix("/UniqueID")) {
            /*tex Ignore |/UniqueID| for subsetted fonts. */
            t1_putline(pdf);
        }
    }
    while (t1_in_eexec == 0);
}
