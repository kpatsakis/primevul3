void GraphicsContext::setPlatformCompositeOperation(CompositeOperator op)
{
    if (paintingDisabled())
        return;

    platformContext()->setXfermodeMode(WebCoreCompositeToSkiaComposite(op));
}
