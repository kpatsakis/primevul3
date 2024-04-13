LayoutUnit RenderBox::overrideHeight() const
{
    return hasOverrideSize() ? overrideSize().height() : height();
}
