int RenderBox::perpendicularContainingBlockLogicalHeight() const
{
    RenderBlock* cb = containingBlock();
    RenderStyle* containingBlockStyle = cb->style();
    Length logicalHeightLength = containingBlockStyle->logicalHeight();
    
    if (!logicalHeightLength.isFixed()) {
        return containingBlockStyle->isHorizontalWritingMode() ? view()->frameView()->visibleHeight() : view()->frameView()->visibleWidth();
    }
    
    return cb->computeContentBoxLogicalHeight(logicalHeightLength.value());
}
