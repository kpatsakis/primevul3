LayoutUnit RenderBox::computeBorderBoxLogicalHeight(LayoutUnit height) const
{
    LayoutUnit bordersPlusPadding = borderAndPaddingLogicalHeight();
    if (style()->boxSizing() == CONTENT_BOX)
        return height + bordersPlusPadding;
    return max(height, bordersPlusPadding);
}
