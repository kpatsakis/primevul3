get_contour_point(hb_font_t *font, void *font_data, hb_codepoint_t glyph,
                     unsigned int point_index, hb_position_t *x,
                     hb_position_t *y, void *user_data)
{
    FT_Face face = font_data;
    int load_flags = FT_LOAD_DEFAULT | FT_LOAD_IGNORE_GLOBAL_ADVANCE_WIDTH
        | FT_LOAD_IGNORE_TRANSFORM;

    if (FT_Load_Glyph(face, glyph, load_flags))
        return false;

    if (point_index >= (unsigned)face->glyph->outline.n_points)
        return false;

    *x = face->glyph->outline.points[point_index].x;
    *y = face->glyph->outline.points[point_index].y;
    return true;
}
