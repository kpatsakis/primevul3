get_cached_metrics(struct ass_shaper_metrics_data *metrics, FT_Face face,
                   hb_codepoint_t unicode, hb_codepoint_t glyph)
{
    GlyphMetricsHashValue *val;
    metrics->hash_key.glyph_index = glyph;
    if (ass_cache_get(metrics->metrics_cache, &metrics->hash_key, &val)) {
        if (val->metrics.width >= 0)
            return val;
        ass_cache_dec_ref(val);
        return NULL;
    }
    if (!val)
        return NULL;

    int load_flags = FT_LOAD_DEFAULT | FT_LOAD_IGNORE_GLOBAL_ADVANCE_WIDTH
        | FT_LOAD_IGNORE_TRANSFORM;

    if (FT_Load_Glyph(face, glyph, load_flags)) {
        val->metrics.width = -1;
        ass_cache_commit(val, 1);
        ass_cache_dec_ref(val);
        return NULL;
    }

    memcpy(&val->metrics, &face->glyph->metrics, sizeof(FT_Glyph_Metrics));

    if (metrics->vertical && unicode >= VERTICAL_LOWER_BOUND)
        val->metrics.horiAdvance = val->metrics.vertAdvance;

    ass_cache_commit(val, 1);
    return val;
}
