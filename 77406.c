void GraphicsContext::setPlatformFillColor(const Color& color, ColorSpace colorSpace)
{
    if (paintingDisabled())
        return;

    platformContext()->setFillColor(color.rgb());
}
