void GraphicsContext::addInnerRoundedRectClip(const IntRect& rect, int thickness)
{
    if (paintingDisabled())
        return;

    SkRect r(rect);
    if (!isRectSkiaSafe(getCTM(), r))
        return;

    SkPath path;
    path.addOval(r, SkPath::kCW_Direction);
    if (2 * thickness < rect.width() && 2 * thickness < rect.height()) {
        r.inset(SkIntToScalar(thickness + 1), SkIntToScalar(thickness + 1));
        path.addOval(r, SkPath::kCCW_Direction);
    }
    platformContext()->clipPathAntiAliased(path);
}
