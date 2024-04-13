static LayoutUnit logicalHeightForLine(const RenderBlock* block, bool isFirstLine, LayoutUnit replacedHeight = 0)
{
    if (!block->document().inNoQuirksMode() && replacedHeight)
        return replacedHeight;

    if (!(block->style(isFirstLine)->lineBoxContain() & LineBoxContainBlock))
        return 0;

    return max<LayoutUnit>(replacedHeight, block->lineHeight(isFirstLine, block->isHorizontalWritingMode() ? HorizontalLine : VerticalLine, PositionOfInteriorLineBoxes));
}
