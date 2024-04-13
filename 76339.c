void InlineTextBox::paintTextMatchMarker(GraphicsContext* pt, int tx, int ty, const DocumentMarker& marker, RenderStyle* style, const Font& font)
{
    int y = selectionTop();
    int h = selectionHeight();
    
    int sPos = max(marker.startOffset - m_start, (unsigned)0);
    int ePos = min(marker.endOffset - m_start, (unsigned)m_len);    
    TextRun run(textRenderer()->text()->characters() + m_start, m_len, textRenderer()->allowTabs(), textPos(), m_toAdd, !isLeftToRightDirection(), m_dirOverride || style->visuallyOrdered());
    
    IntRect markerRect = enclosingIntRect(font.selectionRectForText(run, IntPoint(m_x, y), h, sPos, ePos));
    markerRect = renderer()->localToAbsoluteQuad(FloatRect(markerRect)).enclosingBoundingBox();
    renderer()->document()->markers()->setRenderedRectForMarker(renderer()->node(), marker, markerRect);
    
    if (renderer()->frame()->editor()->markedTextMatchesAreHighlighted()) {
        Color color = marker.activeMatch ?
            renderer()->theme()->platformActiveTextSearchHighlightColor() :
            renderer()->theme()->platformInactiveTextSearchHighlightColor();
        pt->save();
        updateGraphicsContext(pt, color, color, 0, style->colorSpace());  // Don't draw text at all!
        pt->clip(IntRect(tx + m_x, ty + y, m_logicalWidth, h));
        pt->drawHighlightForText(font, run, IntPoint(m_x + tx, y + ty), h, color, style->colorSpace(), sPos, ePos);
        pt->restore();
    }
}
