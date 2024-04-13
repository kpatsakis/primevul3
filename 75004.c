int InlineFlowBox::computeUnderAnnotationAdjustment(int allowedPosition) const
{
    int result = 0;
    for (InlineBox* curr = firstChild(); curr; curr = curr->nextOnLine()) {
        if (curr->renderer()->isPositioned())
            continue; // Positioned placeholders don't affect calculations.

        if (curr->isInlineFlowBox())
            result = max(result, static_cast<InlineFlowBox*>(curr)->computeUnderAnnotationAdjustment(allowedPosition));

        if (curr->isInlineTextBox()) {
            RenderStyle* style = curr->renderer()->style(m_firstLine);
            if (style->textEmphasisMark() != TextEmphasisMarkNone && style->textEmphasisPosition() == TextEmphasisPositionUnder) {
                if (!style->isFlippedLinesWritingMode()) {
                    int bottomOfEmphasisMark = curr->logicalBottom() + style->font().emphasisMarkHeight(style->textEmphasisMarkString());
                    result = max(result, bottomOfEmphasisMark - allowedPosition);
                } else {
                    int topOfEmphasisMark = curr->logicalTop() - style->font().emphasisMarkHeight(style->textEmphasisMarkString());
                    result = max(result, allowedPosition - topOfEmphasisMark);
                }
            }
        }
    }
    return result;
}
