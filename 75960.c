void RenderBox::computeInlineDirectionMargins(RenderBlock* containingBlock, int containerWidth, int childWidth)
{
    const RenderStyle* containingBlockStyle = containingBlock->style();
    Length marginStartLength = style()->marginStartUsing(containingBlockStyle);
    Length marginEndLength = style()->marginEndUsing(containingBlockStyle);

    if (isFloating() || isInline()) {
        containingBlock->setMarginStartForChild(this, marginStartLength.calcMinValue(containerWidth));
        containingBlock->setMarginEndForChild(this, marginEndLength.calcMinValue(containerWidth));
        return;
    }

    if ((marginStartLength.isAuto() && marginEndLength.isAuto() && childWidth < containerWidth)
        || (!marginStartLength.isAuto() && !marginEndLength.isAuto() && containingBlock->style()->textAlign() == WEBKIT_CENTER)) {
        containingBlock->setMarginStartForChild(this, max(0, (containerWidth - childWidth) / 2));
        containingBlock->setMarginEndForChild(this, containerWidth - childWidth - containingBlock->marginStartForChild(this));
        return;
    } 
    
    if (marginEndLength.isAuto() && childWidth < containerWidth) {
        containingBlock->setMarginStartForChild(this, marginStartLength.calcValue(containerWidth));
        containingBlock->setMarginEndForChild(this, containerWidth - childWidth - containingBlock->marginStartForChild(this));
        return;
    } 
    
    bool pushToEndFromTextAlign = !marginEndLength.isAuto() && ((!containingBlockStyle->isLeftToRightDirection() && containingBlockStyle->textAlign() == WEBKIT_LEFT)
        || (containingBlockStyle->isLeftToRightDirection() && containingBlockStyle->textAlign() == WEBKIT_RIGHT));
    if ((marginStartLength.isAuto() && childWidth < containerWidth) || pushToEndFromTextAlign) {
        containingBlock->setMarginEndForChild(this, marginEndLength.calcValue(containerWidth));
        containingBlock->setMarginStartForChild(this, containerWidth - childWidth - containingBlock->marginEndForChild(this));
        return;
    } 
    
    containingBlock->setMarginStartForChild(this, marginStartLength.calcMinValue(containerWidth));
    containingBlock->setMarginEndForChild(this, marginEndLength.calcMinValue(containerWidth));
}
