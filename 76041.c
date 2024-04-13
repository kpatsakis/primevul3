void RenderBox::setOverrideSize(const LayoutSize& size)
{
    if (!gOverrideSizeMap)
        gOverrideSizeMap = new OverrideSizeMap();
    setHasOverrideSize(true);
    gOverrideSizeMap->set(this, size);
}
