static void destroy_t1_glyph_tree(struct avl_table *gl_tree)
{
    avl_destroy(gl_tree, NULL);
}
