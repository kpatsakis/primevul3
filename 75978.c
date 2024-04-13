LayoutUnit RenderBox::computeReplacedLogicalWidthUsing(Length logicalWidth) const
{
    switch (logicalWidth.type()) {
        case Fixed:
            return computeContentBoxLogicalWidth(logicalWidth.value());
        case Percent: {
            const LayoutUnit cw = isPositioned() ? containingBlockLogicalWidthForPositioned(toRenderBoxModelObject(container())) : containingBlockLogicalWidthForContent();
            if (cw > 0)
                return computeContentBoxLogicalWidth(logicalWidth.calcMinValue(cw));
        }
        default:
            return intrinsicLogicalWidth();
     }
}
