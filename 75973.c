LayoutUnit RenderBox::computeReplacedLogicalHeight() const
{
    return computeReplacedLogicalHeightRespectingMinMaxHeight(computeReplacedLogicalHeightUsing(style()->logicalHeight()));
}
