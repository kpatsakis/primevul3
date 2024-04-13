LayoutSize RenderBox::overrideSize() const
{
    if (!hasOverrideSize())
        return LayoutSize(-1, -1);
    return gOverrideSizeMap->get(this);
}
