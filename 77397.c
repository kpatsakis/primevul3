void GraphicsContext::rotate(float angleInRadians)
{
    if (paintingDisabled())
        return;

    platformContext()->canvas()->rotate(WebCoreFloatToSkScalar(
        angleInRadians * (180.0f / 3.14159265f)));
}
