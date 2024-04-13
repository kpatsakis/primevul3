bool RenderBox::includeHorizontalScrollbarSize() const
{
    return hasOverflowClip() && !layer()->hasOverlayScrollbars()
        && (style()->overflowX() == OSCROLL || style()->overflowX() == OAUTO);
}
