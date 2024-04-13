void GraphicsContext::strokeRect(const FloatRect& rect, float lineWidth)
{
    if (paintingDisabled())
        return;

    if (!isRectSkiaSafe(getCTM(), rect))
        return;

    SkPaint paint;
    platformContext()->setupPaintForStroking(&paint, 0, 0);
    paint.setStrokeWidth(WebCoreFloatToSkScalar(lineWidth));
    SkRect r(rect);
    bool validW = r.width() > 0;
    bool validH = r.height() > 0;
    SkCanvas* canvas = platformContext()->canvas();
    if (validW && validH)
        canvas->drawRect(r, paint);
    else if (validW || validH) {
        SkPath path;
        path.moveTo(r.fLeft, r.fTop);
        path.lineTo(r.fRight, r.fBottom);
        path.close();
        canvas->drawPath(path, paint);
    }
}
