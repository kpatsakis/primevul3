void GraphicsContext::setPlatformTextDrawingMode(TextDrawingModeFlags mode)
{
    if (paintingDisabled())
        return;

    platformContext()->setTextDrawingMode(mode);
}
