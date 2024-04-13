LayoutPoint RenderBox::flipForWritingModeIncludingColumns(const LayoutPoint& point) const
{
    if (!hasColumns() || !style()->isFlippedBlocksWritingMode())
        return flipForWritingMode(point);
    return toRenderBlock(this)->flipForWritingModeIncludingColumns(point);
}
