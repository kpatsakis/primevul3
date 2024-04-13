shape_harfbuzz_process_run(GlyphInfo *glyphs, hb_buffer_t *buf, int offset)
{
    int j;
    int num_glyphs = hb_buffer_get_length(buf);
    hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(buf, NULL);
    hb_glyph_position_t *pos    = hb_buffer_get_glyph_positions(buf, NULL);

    for (j = 0; j < num_glyphs; j++) {
        unsigned idx = glyph_info[j].cluster + offset;
        GlyphInfo *info = glyphs + idx;
        GlyphInfo *root = info;

        if (info->skip == 0) {
            while (info->next)
                info = info->next;
            info->next = malloc(sizeof(GlyphInfo));
            if (info->next) {
                memcpy(info->next, info, sizeof(GlyphInfo));
                ass_cache_inc_ref(info->font);
                info = info->next;
                info->next = NULL;
            }
        }

        info->skip = 0;
        info->glyph_index = glyph_info[j].codepoint;
        info->offset.x    = pos[j].x_offset * info->scale_x;
        info->offset.y    = -pos[j].y_offset * info->scale_y;
        info->advance.x   = pos[j].x_advance * info->scale_x;
        info->advance.y   = -pos[j].y_advance * info->scale_y;

        root->cluster_advance.x += info->advance.x;
        root->cluster_advance.y += info->advance.y;
    }
}
