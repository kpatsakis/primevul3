void GraphicsContext::clipConvexPolygon(size_t numPoints, const FloatPoint* points, bool antialiased)
{
    if (paintingDisabled())
        return;

    if (numPoints <= 1)
        return;

    SkPath path;
    if (!isPathSkiaSafe(getCTM(), path))
        return;

    setPathFromConvexPoints(&path, numPoints, points);
    if (antialiased)
        platformContext()->clipPathAntiAliased(path);
    else
        platformContext()->canvas()->clipPath(path);
}
