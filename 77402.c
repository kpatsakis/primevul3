void GraphicsContext::setCTM(const AffineTransform& affine)
{
    if (paintingDisabled())
        return;

    platformContext()->canvas()->setMatrix(affine);
}
