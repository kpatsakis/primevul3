LayoutUnit RenderBox::computeReplacedLogicalHeightRespectingMinMaxHeight(LayoutUnit logicalHeight) const
{
    LayoutUnit minLogicalHeight = computeReplacedLogicalHeightUsing(style()->logicalMinHeight());
    LayoutUnit maxLogicalHeight = style()->logicalMaxHeight().isUndefined() ? logicalHeight : computeReplacedLogicalHeightUsing(style()->logicalMaxHeight());
    return max(minLogicalHeight, min(logicalHeight, maxLogicalHeight));
}
