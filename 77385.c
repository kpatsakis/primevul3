void GraphicsContext::fillRect(const FloatRect& rect)
{
    if (paintingDisabled())
        return;

    SkRect r = rect;
    if (!isRectSkiaSafe(getCTM(), r)) {
        ClipRectToCanvas(*platformContext()->canvas(), r, &r);
    }

    platformContext()->save();

    SkPaint paint;
    platformContext()->setupPaintForFilling(&paint);
    platformContext()->canvas()->drawRect(r, paint);

    platformContext()->restore();
}
