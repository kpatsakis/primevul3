cached_h_advance(hb_font_t *font, void *font_data, hb_codepoint_t glyph,
                 void *user_data)
{
    FT_Face face = font_data;
    struct ass_shaper_metrics_data *metrics_priv = user_data;
    GlyphMetricsHashValue *metrics = get_cached_metrics(metrics_priv, face, 0, glyph);
    if (!metrics)
        return 0;

    hb_position_t advance = metrics->metrics.horiAdvance;
    ass_cache_dec_ref(metrics);
    return advance;
}
