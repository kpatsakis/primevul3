void GraphicsContext::savePlatformState()
{
    if (paintingDisabled())
        return;

    platformContext()->save();
}
