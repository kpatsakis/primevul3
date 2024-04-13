void GraphicsContext::clip(const Path& path)
{
    if (paintingDisabled())
        return;

    const SkPath& p = *path.platformPath();
    if (!isPathSkiaSafe(getCTM(), p))
        return;

    platformContext()->clipPathAntiAliased(p);
}
