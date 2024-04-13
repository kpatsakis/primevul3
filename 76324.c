void InlineTextBox::computeRectForReplacementMarker(int /*tx*/, int /*ty*/, const DocumentMarker& marker, RenderStyle* style, const Font& font)
{
    int y = selectionTop();
    int h = selectionHeight();
    
    int sPos = max(marker.startOffset - m_start, (unsigned)0);
    int ePos = min(marker.endOffset - m_start, (unsigned)m_len);    
    TextRun run(textRenderer()->text()->characters() + m_start, m_len, textRenderer()->allowTabs(), textPos(), m_toAdd, !isLeftToRightDirection(), m_dirOverride || style->visuallyOrdered());
    IntPoint startPoint = IntPoint(m_x, y);
    
    IntRect markerRect = enclosingIntRect(font.selectionRectForText(run, startPoint, h, sPos, ePos));
    markerRect = renderer()->localToAbsoluteQuad(FloatRect(markerRect)).enclosingBoundingBox();
    renderer()->document()->markers()->setRenderedRectForMarker(renderer()->node(), marker, markerRect);
}
