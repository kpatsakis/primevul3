bool RenderBox::includeVerticalScrollbarSize() const
{
    return hasOverflowClip() && !layer()->hasOverlayScrollbars()
        && (style()->overflowY() == OSCROLL || style()->overflowY() == OAUTO);
}
