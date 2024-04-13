LayoutUnit RenderBox::computeLogicalHeightUsing(const Length& h)
{
    LayoutUnit logicalHeight = -1;
    if (!h.isAuto()) {
        if (h.isFixed())
            logicalHeight = h.value();
        else if (h.isPercent())
            logicalHeight = computePercentageLogicalHeight(h);
        if (logicalHeight != -1) {
            logicalHeight = computeBorderBoxLogicalHeight(logicalHeight);
            return logicalHeight;
        }
    }
    return logicalHeight;
}
