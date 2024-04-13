boolean t1_subset_2(char *fontfile, unsigned char *g, char *extraGlyphs)
{
    int i;
    for (i = 0; i < 256; i++)
        ext_glyph_names[i] = (char*) notdef;
    grid = g;
    cur_file_name = fontfile;
    hexline_length = 0;
    dvips_extra_charset = extraGlyphs;
    writet1();
    for (i = 0; i < 256; i++)
        if (ext_glyph_names[i] != notdef)
            free(ext_glyph_names[i]);
    return 1; /* note:  there *is* no unsuccessful return */
}
