bool RenderBox::needsPreferredWidthsRecalculation() const
{
    return style()->paddingStart().isPercent() || style()->paddingEnd().isPercent();
}
