void InlineTextBox::paintCompositionBackground(GraphicsContext* context, int tx, int ty, RenderStyle* style, const Font& font, int startPos, int endPos)
{
    int offset = m_start;
    int sPos = max(startPos - offset, 0);
    int ePos = min(endPos - offset, (int)m_len);

    if (sPos >= ePos)
        return;

    context->save();

    Color c = Color(225, 221, 85);
    
    updateGraphicsContext(context, c, c, 0, style->colorSpace()); // Don't draw text at all!

    int y = selectionTop();
    int h = selectionHeight();
    context->drawHighlightForText(font, TextRun(textRenderer()->text()->characters() + m_start, m_len, textRenderer()->allowTabs(), textPos(), m_toAdd,
                                  !isLeftToRightDirection(), m_dirOverride || style->visuallyOrdered()),
                                  IntPoint(m_x + tx, y + ty), h, c, style->colorSpace(), sPos, ePos);
    context->restore();
}
