void GraphicsContext::fillRoundedRect(const IntRect& rect,
                                      const IntSize& topLeft,
                                      const IntSize& topRight,
                                      const IntSize& bottomLeft,
                                      const IntSize& bottomRight,
                                      const Color& color,
                                      ColorSpace colorSpace)
{
    if (paintingDisabled())
        return;

    SkRect r = rect;
    if (!isRectSkiaSafe(getCTM(), r))
        ClipRectToCanvas(*platformContext()->canvas(), r, &r);

    if (topLeft.width() + topRight.width() > rect.width()
            || bottomLeft.width() + bottomRight.width() > rect.width()
            || topLeft.height() + bottomLeft.height() > rect.height()
            || topRight.height() + bottomRight.height() > rect.height()) {
        fillRect(rect, color, colorSpace);
        return;
    }

    SkPath path;
    addCornerArc(&path, r, topRight, 270);
    addCornerArc(&path, r, bottomRight, 0);
    addCornerArc(&path, r, bottomLeft, 90);
    addCornerArc(&path, r, topLeft, 180);

    SkPaint paint;
    platformContext()->setupPaintForFilling(&paint);
    paint.setColor(color.rgb());
    platformContext()->canvas()->drawPath(path, paint);
}
