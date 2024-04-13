void GraphicsContext::drawRect(const IntRect& rect)
{
    if (paintingDisabled())
        return;

    SkRect r = rect;
    if (!isRectSkiaSafe(getCTM(), r)) {
        ClipRectToCanvas(*platformContext()->canvas(), r, &r);
    }

    platformContext()->drawRect(r);
}
