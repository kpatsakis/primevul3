void RenderBox::computeBlockDirectionMargins(RenderBlock* containingBlock)
{
    if (isTableCell()) {
        setMarginBefore(0);
        setMarginAfter(0);
        return;
    }

    int cw = containingBlockLogicalWidthForContent();

    RenderStyle* containingBlockStyle = containingBlock->style();
    containingBlock->setMarginBeforeForChild(this, style()->marginBeforeUsing(containingBlockStyle).calcMinValue(cw));
    containingBlock->setMarginAfterForChild(this, style()->marginAfterUsing(containingBlockStyle).calcMinValue(cw));
}
