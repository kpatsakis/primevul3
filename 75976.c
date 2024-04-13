LayoutUnit RenderBox::computeReplacedLogicalWidth(bool includeMaxWidth) const
{
    return computeReplacedLogicalWidthRespectingMinMaxWidth(computeReplacedLogicalWidthUsing(style()->logicalWidth()), includeMaxWidth);
}
