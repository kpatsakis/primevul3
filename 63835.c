static void t1_mark_glyphs(void)
{
    char *glyph;
    struct avl_traverser t;
    cs_entry *ptr;
    if (t1_synthetic || fd_cur->all_glyphs) {   /* mark everything */
        if (cs_tab != NULL)
            for (ptr = cs_tab; ptr < cs_ptr; ptr++)
                if (ptr->valid)
                    ptr->used = true;
        if (subr_tab != NULL) {
            for (ptr = subr_tab; ptr - subr_tab < subr_size; ptr++)
                if (ptr->valid)
                    ptr->used = true;
            subr_max = subr_size - 1;
        }
        return;
    }
    mark_cs(notdef);
    avl_t_init(&t, fd_cur->gl_tree);
    for (glyph = (char *) avl_t_first(&t, fd_cur->gl_tree); glyph != NULL;
         glyph = (char *) avl_t_next(&t)) {
        mark_cs(glyph);
    }
    if (subr_tab != NULL)
        for (subr_max = -1, ptr = subr_tab; ptr - subr_tab < subr_size; ptr++)
            if (ptr->used && ptr - subr_tab > subr_max)
                subr_max = ptr - subr_tab;
}
