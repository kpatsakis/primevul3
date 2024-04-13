int RenderBox::containingBlockLogicalHeightForPositioned(const RenderBoxModelObject* containingBlock, bool checkForPerpendicularWritingMode) const
{
    if (checkForPerpendicularWritingMode && containingBlock->isHorizontalWritingMode() != isHorizontalWritingMode())
        return containingBlockLogicalWidthForPositioned(containingBlock, false);

    if (containingBlock->isBox())
        return toRenderBox(containingBlock)->clientLogicalHeight();
        
    ASSERT(containingBlock->isRenderInline() && containingBlock->isRelPositioned());

    const RenderInline* flow = toRenderInline(containingBlock);
    InlineFlowBox* first = flow->firstLineBox();
    InlineFlowBox* last = flow->lastLineBox();

    if (!first || !last)
        return 0;

    int heightResult;
    IntRect boundingBox = flow->linesBoundingBox();
    if (containingBlock->isHorizontalWritingMode())
        heightResult = boundingBox.height();
    else
        heightResult = boundingBox.width();
    heightResult -= (containingBlock->borderBefore() + containingBlock->borderAfter());
    return heightResult;
}
