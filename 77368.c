void GraphicsContext::clearRect(const FloatRect& rect)
{
    if (paintingDisabled())
        return;

    SkRect r = rect;
    if (!isRectSkiaSafe(getCTM(), r))
        ClipRectToCanvas(*platformContext()->canvas(), r, &r);

    SkPaint paint;
    platformContext()->setupPaintForFilling(&paint);
    paint.setXfermodeMode(SkXfermode::kClear_Mode);
    platformContext()->canvas()->drawRect(r, paint);
}
