int InlineFlowBox::getFlowSpacingLogicalWidth()
{
    int totWidth = marginBorderPaddingLogicalLeft() + marginBorderPaddingLogicalRight();
    for (InlineBox* curr = firstChild(); curr; curr = curr->nextOnLine()) {
        if (curr->isInlineFlowBox())
            totWidth += static_cast<InlineFlowBox*>(curr)->getFlowSpacingLogicalWidth();
    }
    return totWidth;
}
