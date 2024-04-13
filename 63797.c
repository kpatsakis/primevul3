static struct avl_table *create_t1_glyph_tree(char **glyph_names)
{
    int i;
    void **aa;
    static struct avl_table *gl_tree;
    gl_tree = avl_create(comp_t1_glyphs, NULL, &avl_xallocator);
    for (i = 0; i < 256; i++) {
        if (glyph_names[i] != notdef &&
            (char **) avl_find(gl_tree, &glyph_names[i]) == NULL) {
            /*tex No |strdup| here, just point to the |glyph_names| array members. */
            aa = avl_probe(gl_tree, &glyph_names[i]);
            if (aa == NULL) {
                /*tex Is this a problem? */
            }
        }
    }
    return gl_tree;
}
