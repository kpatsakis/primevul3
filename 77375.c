 void GraphicsContext::drawConvexPolygon(size_t numPoints,
                                        const FloatPoint* points,
                                        bool shouldAntialias)
{
    if (paintingDisabled())
        return;

    if (numPoints <= 1)
        return;

    SkPath path;
    setPathFromConvexPoints(&path, numPoints, points);

    if (!isPathSkiaSafe(getCTM(), path))
        return;

    SkPaint paint;
    platformContext()->setupPaintForFilling(&paint);
    paint.setAntiAlias(shouldAntialias);
    platformContext()->canvas()->drawPath(path, paint);

    if (strokeStyle() != NoStroke) {
        paint.reset();
        platformContext()->setupPaintForStroking(&paint, 0, 0);
        platformContext()->canvas()->drawPath(path, paint);
    }
}
