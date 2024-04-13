static inline float measureHyphenWidth(RenderText* renderer, const Font& font)
{
    RenderStyle* style = renderer->style();
    return font.width(RenderBlock::constructTextRun(renderer, font, style->hyphenString().string(), style));
}
