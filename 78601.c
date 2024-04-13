static ALWAYS_INLINE float textWidth(RenderText* text, unsigned from, unsigned len, const Font& font, float xPos, bool isFixedPitch, bool collapseWhiteSpace, HashSet<const SimpleFontData*>* fallbackFonts = 0, TextLayout* layout = 0)
{
    GlyphOverflow glyphOverflow;
    if (isFixedPitch || (!from && len == text->textLength()) || text->style()->hasTextCombine())
        return text->width(from, len, font, xPos, fallbackFonts, &glyphOverflow);

    if (layout)
        return Font::width(*layout, from, len, fallbackFonts);

    TextRun run = RenderBlock::constructTextRun(text, font, text, from, len, text->style());
    run.setCharactersLength(text->textLength() - from);
    ASSERT(run.charactersLength() >= run.length());

    run.setCharacterScanForCodePath(!text->canUseSimpleFontCodePath());
    run.setTabSize(!collapseWhiteSpace, text->style()->tabSize());
    run.setXPos(xPos);
    return font.width(run, fallbackFonts, &glyphOverflow);
}
