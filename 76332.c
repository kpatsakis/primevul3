int InlineTextBox::offsetForPosition(int _x, bool includePartialGlyphs) const
{
    if (isLineBreak())
        return 0;

    RenderText* text = toRenderText(renderer());
    RenderStyle* style = text->style(m_firstLine);
    const Font* f = &style->font();
    return f->offsetForPosition(TextRun(textRenderer()->text()->characters() + m_start, m_len, textRenderer()->allowTabs(), textPos(), m_toAdd, !isLeftToRightDirection(), m_dirOverride || style->visuallyOrdered()),
                                _x - m_x, includePartialGlyphs);
}
