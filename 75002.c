int InlineFlowBox::computeOverAnnotationAdjustment(int allowedPosition) const
{
    int result = 0;
    for (InlineBox* curr = firstChild(); curr; curr = curr->nextOnLine()) {
        if (curr->renderer()->isPositioned())
            continue; // Positioned placeholders don't affect calculations.
        
        if (curr->isInlineFlowBox())
            result = max(result, static_cast<InlineFlowBox*>(curr)->computeOverAnnotationAdjustment(allowedPosition));
        
        if (curr->renderer()->isReplaced() && curr->renderer()->isRubyRun()) {
            RenderRubyRun* rubyRun = static_cast<RenderRubyRun*>(curr->renderer());
            RenderRubyText* rubyText = rubyRun->rubyText();
            if (!rubyText)
                continue;
            
            if (!rubyRun->style()->isFlippedLinesWritingMode()) {
                int topOfFirstRubyTextLine = rubyText->logicalTop() + (rubyText->firstRootBox() ? rubyText->firstRootBox()->lineTop() : 0);
                if (topOfFirstRubyTextLine >= 0)
                    continue;
                topOfFirstRubyTextLine += curr->logicalTop();
                result = max(result, allowedPosition - topOfFirstRubyTextLine);
            } else {
                int bottomOfLastRubyTextLine = rubyText->logicalTop() + (rubyText->lastRootBox() ? rubyText->lastRootBox()->lineBottom() : rubyText->logicalHeight());
                if (bottomOfLastRubyTextLine <= curr->logicalHeight())
                    continue;
                bottomOfLastRubyTextLine += curr->logicalTop();
                result = max(result, bottomOfLastRubyTextLine - allowedPosition);
            }
        }

        if (curr->isInlineTextBox()) {
            RenderStyle* style = curr->renderer()->style(m_firstLine);
            TextEmphasisPosition emphasisMarkPosition;
            if (style->textEmphasisMark() != TextEmphasisMarkNone && static_cast<InlineTextBox*>(curr)->getEmphasisMarkPosition(style, emphasisMarkPosition) && emphasisMarkPosition == TextEmphasisPositionOver) {
                if (!style->isFlippedLinesWritingMode()) {
                    int topOfEmphasisMark = curr->logicalTop() - style->font().emphasisMarkHeight(style->textEmphasisMarkString());
                    result = max(result, allowedPosition - topOfEmphasisMark);
                } else {
                    int bottomOfEmphasisMark = curr->logicalBottom() + style->font().emphasisMarkHeight(style->textEmphasisMarkString());
                    result = max(result, bottomOfEmphasisMark - allowedPosition);
                }
            }
        }
    }
    return result;
}
