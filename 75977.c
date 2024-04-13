LayoutUnit RenderBox::computeReplacedLogicalWidthRespectingMinMaxWidth(LayoutUnit logicalWidth, bool includeMaxWidth) const
{
    LayoutUnit minLogicalWidth = computeReplacedLogicalWidthUsing(style()->logicalMinWidth());
    LayoutUnit maxLogicalWidth = !includeMaxWidth || style()->logicalMaxWidth().isUndefined() ? logicalWidth : computeReplacedLogicalWidthUsing(style()->logicalMaxWidth());
    return max(minLogicalWidth, min(logicalWidth, maxLogicalWidth));
}
