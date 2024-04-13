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
