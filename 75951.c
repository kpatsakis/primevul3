void RenderBox::clearOverrideSize()
{
    if (!hasOverrideSize())
        return;
    setHasOverrideSize(false);
    gOverrideSizeMap->remove(this);
}
