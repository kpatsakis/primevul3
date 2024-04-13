void GraphicsContext::scale(const FloatSize& size)
{
    if (paintingDisabled())
        return;

    platformContext()->canvas()->scale(WebCoreFloatToSkScalar(size.width()),
        WebCoreFloatToSkScalar(size.height()));
}
