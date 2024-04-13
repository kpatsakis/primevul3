get_glyph(hb_font_t *font, void *font_data, hb_codepoint_t unicode,
          hb_codepoint_t variation, hb_codepoint_t *glyph, void *user_data)
{
    FT_Face face = font_data;
    struct ass_shaper_metrics_data *metrics_priv = user_data;

    if (variation)
        *glyph = FT_Face_GetCharVariantIndex(face, ass_font_index_magic(face, unicode), variation);
    else
        *glyph = FT_Get_Char_Index(face, ass_font_index_magic(face, unicode));
    if (!*glyph)
        return false;

    GlyphMetricsHashValue *metrics = get_cached_metrics(metrics_priv, face, unicode, *glyph);
    ass_cache_dec_ref(metrics);
    return true;
}
