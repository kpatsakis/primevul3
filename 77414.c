void GraphicsContext::strokePath(const Path& pathToStroke)
{
    if (paintingDisabled())
        return;

    SkPath path = *pathToStroke.platformPath();
    if (!isPathSkiaSafe(getCTM(), path))
        return;

    SkPaint paint;
    platformContext()->setupPaintForStroking(&paint, 0, 0);
    platformContext()->canvas()->drawPath(path, paint);
}
