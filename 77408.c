void GraphicsContext::setPlatformShouldAntialias(bool enable)
{
    if (paintingDisabled())
        return;

    platformContext()->setUseAntialiasing(enable);
}
