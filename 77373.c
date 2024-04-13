void GraphicsContext::concatCTM(const AffineTransform& affine)
{
    if (paintingDisabled())
        return;

    platformContext()->canvas()->concat(affine);
}
