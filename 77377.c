void GraphicsContext::drawFocusRing(const Vector<IntRect>& rects, int width, int offset, const Color& color)
{
    if (paintingDisabled())
        return;

    unsigned rectCount = rects.size();
    if (!rectCount)
        return;

    SkRegion focusRingRegion;
    const int focusRingOutset = getFocusRingOutset(offset);
    for (unsigned i = 0; i < rectCount; i++) {
        SkIRect r = rects[i];
        r.inset(-focusRingOutset, -focusRingOutset);
        focusRingRegion.op(r, SkRegion::kUnion_Op);
    }

    SkPath path;
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kStroke_Style);

    paint.setColor(color.rgb());
    focusRingRegion.getBoundaryPath(&path);
    SkCanvas* canvas = platformContext()->canvas();
    drawOuterPath(canvas, path, paint, width);
    drawInnerPath(canvas, path, paint, width);
}
