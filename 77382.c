void GraphicsContext::endPlatformTransparencyLayer()
{
    if (paintingDisabled())
        return;
    platformContext()->canvas()->restore();
}
