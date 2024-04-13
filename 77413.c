void GraphicsContext::strokeArc(const IntRect& r, int startAngle, int angleSpan)
{
    if (paintingDisabled())
        return;

    SkPaint paint;
    SkRect oval = r;
    if (strokeStyle() == NoStroke) {
        platformContext()->setupPaintForFilling(&paint);
        paint.setStyle(SkPaint::kStroke_Style);
        paint.setStrokeWidth(WebCoreFloatToSkScalar(strokeThickness()));
    } else
        platformContext()->setupPaintForStroking(&paint, 0, 0);

    startAngle = fastMod(startAngle, 360);
    angleSpan = fastMod(angleSpan, 360);

    SkPath path;
    path.addArc(oval, SkIntToScalar(-startAngle), SkIntToScalar(-angleSpan));
    if (!isPathSkiaSafe(getCTM(), path))
        return;
    platformContext()->canvas()->drawPath(path, paint);
}
