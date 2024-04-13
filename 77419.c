HB_Bool HB_MyanmarShape(HB_ShaperItem *item)
{
    HB_Bool openType = FALSE;
    unsigned short *logClusters = item->log_clusters;

    HB_ShaperItem syllable = *item;
    int first_glyph = 0;

    int sstart = item->item.pos;
    int end = sstart + item->item.length;
    int i = 0;

    assert(item->item.script == HB_Script_Myanmar);
#ifndef NO_OPENTYPE
    openType = HB_SelectScript(item, myanmar_features);
#endif

    MMDEBUG("myanmar_shape: from %d length %d", item->item.pos, item->item.length);
    while (sstart < end) {
        HB_Bool invalid;
        int send = myanmar_nextSyllableBoundary(item->string, sstart, end, &invalid);
        MMDEBUG("syllable from %d, length %d, invalid=%s", sstart, send-sstart,
               invalid ? "TRUE" : "FALSE");
        syllable.item.pos = sstart;
        syllable.item.length = send-sstart;
        syllable.glyphs = item->glyphs + first_glyph;
        syllable.attributes = item->attributes + first_glyph;
        syllable.advances = item->advances + first_glyph;
        syllable.offsets = item->offsets + first_glyph;
        syllable.num_glyphs = item->num_glyphs - first_glyph;
        if (!myanmar_shape_syllable(openType, &syllable, invalid)) {
            MMDEBUG("syllable shaping failed, syllable requests %d glyphs", syllable.num_glyphs);
            item->num_glyphs += syllable.num_glyphs;
            return FALSE;
        }

        /* fix logcluster array */
        MMDEBUG("syllable:");
        for (i = first_glyph; i < first_glyph + (int)syllable.num_glyphs; ++i)
            MMDEBUG("        %d -> glyph %x", i, item->glyphs[i]);
        MMDEBUG("    logclusters:");
        for (i = sstart; i < send; ++i) {
            MMDEBUG("        %d -> glyph %d", i, first_glyph);
            logClusters[i-item->item.pos] = first_glyph;
        }
        sstart = send;
        first_glyph += syllable.num_glyphs;
    }
    item->num_glyphs = first_glyph;
    return TRUE;
}
