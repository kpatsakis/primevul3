void GraphicsContext::translate(float w, float h)
{
    if (paintingDisabled())
        return;

    platformContext()->canvas()->translate(WebCoreFloatToSkScalar(w),
                                           WebCoreFloatToSkScalar(h));
}
