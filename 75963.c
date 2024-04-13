static void computeLogicalLeftPositionedOffset(int& logicalLeftPos, const RenderBox* child, int logicalWidthValue, const RenderBoxModelObject* containerBlock, int containerLogicalWidth)
{
    if (containerBlock->isHorizontalWritingMode() != child->isHorizontalWritingMode() && containerBlock->style()->isFlippedBlocksWritingMode()) {
        logicalLeftPos = containerLogicalWidth - logicalWidthValue - logicalLeftPos;
        logicalLeftPos += (child->isHorizontalWritingMode() ? containerBlock->borderRight() : containerBlock->borderBottom());
    } else
        logicalLeftPos += (child->isHorizontalWritingMode() ? containerBlock->borderLeft() : containerBlock->borderTop());
}
