LayoutUnit RenderBox::overrideWidth() const
{
    return hasOverrideSize() ? overrideSize().width() : width();
}
