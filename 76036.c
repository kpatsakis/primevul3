int RenderBox::scrollWidth() const
{
    if (hasOverflowClip())
        return layer()->scrollWidth();
    if (style()->isLeftToRightDirection())
        return max(clientWidth(), maxXLayoutOverflow() - borderLeft());
    return clientWidth() - min(0, minXLayoutOverflow() - borderLeft());
}
