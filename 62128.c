cached_extents(hb_font_t *font, void *font_data, hb_codepoint_t glyph,
               hb_glyph_extents_t *extents, void *user_data)
{
    FT_Face face = font_data;
    struct ass_shaper_metrics_data *metrics_priv = user_data;
    GlyphMetricsHashValue *metrics = get_cached_metrics(metrics_priv, face, 0, glyph);
    if (!metrics)
        return false;

    extents->x_bearing = metrics->metrics.horiBearingX;
    extents->y_bearing = metrics->metrics.horiBearingY;
    extents->width     = metrics->metrics.width;
    extents->height    = -metrics->metrics.height;
    ass_cache_dec_ref(metrics);
    return true;
}
