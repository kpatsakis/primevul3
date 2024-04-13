void GraphicsContext::setPlatformStrokeColor(const Color& strokecolor, ColorSpace colorSpace)
{
    if (paintingDisabled())
        return;

    platformContext()->setStrokeColor(strokecolor.rgb());
}
