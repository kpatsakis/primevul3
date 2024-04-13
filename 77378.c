void GraphicsContext::drawLine(const IntPoint& point1, const IntPoint& point2)
{
    if (paintingDisabled())
        return;

    StrokeStyle penStyle = strokeStyle();
    if (penStyle == NoStroke)
        return;

    SkPaint paint;
    if (!isPointSkiaSafe(getCTM(), point1) || !isPointSkiaSafe(getCTM(), point2))
        return;

    FloatPoint p1 = point1;
    FloatPoint p2 = point2;
    bool isVerticalLine = (p1.x() == p2.x());
    int width = roundf(strokeThickness());

    FloatSize disp = p2 - p1;
    int length = SkScalarRound(disp.width() + disp.height());
    platformContext()->setupPaintForStroking(&paint, 0, length);

    if (strokeStyle() == DottedStroke || strokeStyle() == DashedStroke) {

        SkRect r1, r2;
        r1.set(p1.x(), p1.y(), p1.x() + width, p1.y() + width);
        r2.set(p2.x(), p2.y(), p2.x() + width, p2.y() + width);

        if (isVerticalLine) {
            r1.offset(-width / 2, 0);
            r2.offset(-width / 2, -width);
        } else {
            r1.offset(0, -width / 2);
            r2.offset(-width, -width / 2);
        }
        SkPaint fillPaint;
        fillPaint.setColor(paint.getColor());
        platformContext()->canvas()->drawRect(r1, fillPaint);
        platformContext()->canvas()->drawRect(r2, fillPaint);
    }

    adjustLineToPixelBoundaries(p1, p2, width, penStyle);
    SkPoint pts[2] = { (SkPoint)p1, (SkPoint)p2 };

    platformContext()->canvas()->drawPoints(SkCanvas::kLines_PointMode, 2, pts, paint);
}
