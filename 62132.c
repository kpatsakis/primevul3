cached_v_origin(hb_font_t *font, void *font_data, hb_codepoint_t glyph,
                hb_position_t *x, hb_position_t *y, void *user_data)
{
    FT_Face face = font_data;
    struct ass_shaper_metrics_data *metrics_priv = user_data;
    GlyphMetricsHashValue *metrics = get_cached_metrics(metrics_priv, face, 0, glyph);
    if (!metrics)
        return false;

    *x = metrics->metrics.horiBearingX - metrics->metrics.vertBearingX;
    *y = metrics->metrics.horiBearingY - (-metrics->metrics.vertBearingY);
    ass_cache_dec_ref(metrics);
    return true;
}
