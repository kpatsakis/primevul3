static inline void pushShapeContentOverflowBelowTheContentBox(RenderBlock* block, ShapeInsideInfo* shapeInsideInfo, LayoutUnit lineTop, LayoutUnit lineHeight)
{
    ASSERT(shapeInsideInfo);

    LayoutUnit logicalLineBottom = lineTop + lineHeight;
    LayoutUnit shapeLogicalBottom = shapeInsideInfo->shapeLogicalBottom();
    LayoutUnit shapeContainingBlockHeight = shapeInsideInfo->shapeContainingBlockHeight();

    bool isOverflowPositionedAlready = (shapeContainingBlockHeight - shapeInsideInfo->owner()->borderAndPaddingAfter() + lineHeight) <= lineTop;

    if (lineTop < shapeLogicalBottom && shapeLogicalBottom < logicalLineBottom)
        shapeInsideInfo->clearSegments();
    if (logicalLineBottom <= shapeLogicalBottom || !shapeContainingBlockHeight || isOverflowPositionedAlready)
        return;

    LayoutUnit newLogicalHeight = block->logicalHeight() + (shapeContainingBlockHeight - (lineTop + shapeInsideInfo->owner()->borderAndPaddingAfter()));
    block->setLogicalHeight(newLogicalHeight);
}
