void GraphicsContext::clipPath(const Path& pathToClip, WindRule clipRule)
{
    if (paintingDisabled())
        return;

    SkPath path = *pathToClip.platformPath();
    if (!isPathSkiaSafe(getCTM(), path))
        return;

    path.setFillType(clipRule == RULE_EVENODD ? SkPath::kEvenOdd_FillType : SkPath::kWinding_FillType);
    platformContext()->clipPathAntiAliased(path);
}
