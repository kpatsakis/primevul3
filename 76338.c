void InlineTextBox::paintSpellingOrGrammarMarker(GraphicsContext* pt, int tx, int ty, const DocumentMarker& marker, RenderStyle* style, const Font& font, bool grammar)
{
    if (textRenderer()->document()->printing())
        return;

    if (m_truncation == cFullTruncation)
        return;

    int start = 0;                  // start of line to draw, relative to tx
    int width = m_logicalWidth;            // how much line to draw

    bool markerSpansWholeBox = true;
    if (m_start <= (int)marker.startOffset)
        markerSpansWholeBox = false;
    if ((end() + 1) != marker.endOffset)      // end points at the last char, not past it
        markerSpansWholeBox = false;
    if (m_truncation != cNoTruncation)
        markerSpansWholeBox = false;

    if (!markerSpansWholeBox || grammar) {
        int startPosition = max<int>(marker.startOffset - m_start, 0);
        int endPosition = min<int>(marker.endOffset - m_start, m_len);
        
        if (m_truncation != cNoTruncation)
            endPosition = min<int>(endPosition, m_truncation);

        IntPoint startPoint(tx + m_x, ty + selectionTop());
        TextRun run(textRenderer()->text()->characters() + m_start, m_len, textRenderer()->allowTabs(), textPos(), m_toAdd, !isLeftToRightDirection(), m_dirOverride || style->visuallyOrdered());
        int h = selectionHeight();
        
        IntRect markerRect = enclosingIntRect(font.selectionRectForText(run, startPoint, h, startPosition, endPosition));
        start = markerRect.x() - startPoint.x();
        width = markerRect.width();
        
        if (grammar) {
            markerRect.move(-tx, -ty);
            markerRect = renderer()->localToAbsoluteQuad(FloatRect(markerRect)).enclosingBoundingBox();
            renderer()->document()->markers()->setRenderedRectForMarker(renderer()->node(), marker, markerRect);
        }
    }
    
    int lineThickness = cMisspellingLineThickness;
    int baseline = renderer()->style(m_firstLine)->font().ascent();
    int descent = logicalHeight() - baseline;
    int underlineOffset;
    if (descent <= (2 + lineThickness)) {
        underlineOffset = logicalHeight() - lineThickness;
    } else {
        underlineOffset = baseline + 2;
    }
    pt->drawLineForTextChecking(IntPoint(tx + m_x + start, ty + m_y + underlineOffset), width, textCheckingLineStyleForMarkerType(marker.type));
}
