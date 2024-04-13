AffineTransform GraphicsContext::getCTM() const
{
    if (paintingDisabled())
        return AffineTransform();

    const SkMatrix& m = platformContext()->canvas()->getTotalMatrix();
    return AffineTransform(SkScalarToDouble(m.getScaleX()),
                           SkScalarToDouble(m.getSkewY()),
                           SkScalarToDouble(m.getSkewX()),
                           SkScalarToDouble(m.getScaleY()),
                           SkScalarToDouble(m.getTranslateX()),
                           SkScalarToDouble(m.getTranslateY()));
}
