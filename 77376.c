void GraphicsContext::drawEllipse(const IntRect& elipseRect)
{
    if (paintingDisabled())
        return;

    SkRect rect = elipseRect;
    if (!isRectSkiaSafe(getCTM(), rect))
        return;

    SkPaint paint;
    platformContext()->setupPaintForFilling(&paint);
    platformContext()->canvas()->drawOval(rect, paint);

    if (strokeStyle() != NoStroke) {
        paint.reset();
        platformContext()->setupPaintForStroking(&paint, &rect, 0);
        platformContext()->canvas()->drawOval(rect, paint);
    }
}
