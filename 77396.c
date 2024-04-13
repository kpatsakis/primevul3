void GraphicsContext::restorePlatformState()
{
    if (paintingDisabled())
        return;

    platformContext()->restore();
}
