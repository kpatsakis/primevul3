static void computeLogicalTopPositionedOffset(int& logicalTopPos, const RenderBox* child, int logicalHeightValue, const RenderBoxModelObject* containerBlock, int containerLogicalHeight)
{
    if ((child->style()->isFlippedBlocksWritingMode() && child->isHorizontalWritingMode() != containerBlock->isHorizontalWritingMode())
        || (child->style()->isFlippedBlocksWritingMode() != containerBlock->style()->isFlippedBlocksWritingMode() && child->isHorizontalWritingMode() == containerBlock->isHorizontalWritingMode()))
        logicalTopPos = containerLogicalHeight - logicalHeightValue - logicalTopPos;

    if (containerBlock->style()->isFlippedBlocksWritingMode() && child->isHorizontalWritingMode() == containerBlock->isHorizontalWritingMode()) {
        if (child->isHorizontalWritingMode())
            logicalTopPos += containerBlock->borderBottom();
        else
            logicalTopPos += containerBlock->borderRight();
    } else {
        if (child->isHorizontalWritingMode())
            logicalTopPos += containerBlock->borderTop();
        else
            logicalTopPos += containerBlock->borderLeft();
    }
}
