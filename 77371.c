void GraphicsContext::clipOut(const IntRect& rect)
{
    if (paintingDisabled())
        return;

    SkRect r(rect);
    if (!isRectSkiaSafe(getCTM(), r))
        return;

    platformContext()->canvas()->clipRect(r, SkRegion::kDifference_Op);
}
