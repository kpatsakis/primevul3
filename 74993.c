void InlineFlowBox::addTextBoxVisualOverflow(const InlineTextBox* textBox, GlyphOverflowAndFallbackFontsMap& textBoxDataMap, IntRect& logicalVisualOverflow)
{
    RenderStyle* style = renderer()->style(m_firstLine);
    int strokeOverflow = static_cast<int>(ceilf(style->textStrokeWidth() / 2.0f));

    GlyphOverflowAndFallbackFontsMap::iterator it = textBoxDataMap.find(textBox);
    GlyphOverflow* glyphOverflow = it == textBoxDataMap.end() ? 0 : &it->second.second;

    bool isFlippedLine = style->isFlippedLinesWritingMode();

    int topGlyphEdge = glyphOverflow ? (isFlippedLine ? glyphOverflow->bottom : glyphOverflow->top) : 0;
    int bottomGlyphEdge = glyphOverflow ? (isFlippedLine ? glyphOverflow->top : glyphOverflow->bottom) : 0;
    int leftGlyphEdge = glyphOverflow ? glyphOverflow->left : 0;
    int rightGlyphEdge = glyphOverflow ? glyphOverflow->right : 0;

    int topGlyphOverflow = -strokeOverflow - topGlyphEdge;
    int bottomGlyphOverflow = strokeOverflow + bottomGlyphEdge;
    int leftGlyphOverflow = -strokeOverflow - leftGlyphEdge;
    int rightGlyphOverflow = strokeOverflow + rightGlyphEdge;

    TextEmphasisPosition emphasisMarkPosition;
    if (style->textEmphasisMark() != TextEmphasisMarkNone && textBox->getEmphasisMarkPosition(style, emphasisMarkPosition)) {
        int emphasisMarkHeight = style->font().emphasisMarkHeight(style->textEmphasisMarkString());
        if ((emphasisMarkPosition == TextEmphasisPositionOver) == (!style->isFlippedLinesWritingMode()))
            topGlyphOverflow = min(topGlyphOverflow, -emphasisMarkHeight);
        else
            bottomGlyphOverflow = max(bottomGlyphOverflow, emphasisMarkHeight);
    }

    int letterSpacing = min(0, (int)style->font().letterSpacing());
    rightGlyphOverflow -= letterSpacing;

    int textShadowLogicalTop;
    int textShadowLogicalBottom;
    style->getTextShadowBlockDirectionExtent(textShadowLogicalTop, textShadowLogicalBottom);
    
    int childOverflowLogicalTop = min(textShadowLogicalTop + topGlyphOverflow, topGlyphOverflow);
    int childOverflowLogicalBottom = max(textShadowLogicalBottom + bottomGlyphOverflow, bottomGlyphOverflow);
   
    int textShadowLogicalLeft;
    int textShadowLogicalRight;
    style->getTextShadowInlineDirectionExtent(textShadowLogicalLeft, textShadowLogicalRight);
   
    int childOverflowLogicalLeft = min(textShadowLogicalLeft + leftGlyphOverflow, leftGlyphOverflow);
    int childOverflowLogicalRight = max(textShadowLogicalRight + rightGlyphOverflow, rightGlyphOverflow);

    int logicalTopVisualOverflow = min(textBox->logicalTop() + childOverflowLogicalTop, logicalVisualOverflow.y());
    int logicalBottomVisualOverflow = max(textBox->logicalBottom() + childOverflowLogicalBottom, logicalVisualOverflow.maxY());
    int logicalLeftVisualOverflow = min(textBox->pixelSnappedLogicalLeft() + childOverflowLogicalLeft, logicalVisualOverflow.x());
    int logicalRightVisualOverflow = max(textBox->pixelSnappedLogicalRight() + childOverflowLogicalRight, logicalVisualOverflow.maxX());
    
    logicalVisualOverflow = IntRect(logicalLeftVisualOverflow, logicalTopVisualOverflow,
                                    logicalRightVisualOverflow - logicalLeftVisualOverflow, logicalBottomVisualOverflow - logicalTopVisualOverflow);
}
