static inline void drawOuterPath(SkCanvas* canvas, const SkPath& path, SkPaint& paint, int width)
{
#if PLATFORM(CHROMIUM) && OS(DARWIN)
    paint.setAlpha(64);
    paint.setStrokeWidth(width);
    paint.setPathEffect(new SkCornerPathEffect((width - 1) * 0.5f))->unref();
#else
    paint.setStrokeWidth(1);
    paint.setPathEffect(new SkCornerPathEffect(1))->unref();
#endif
    canvas->drawPath(path, paint);
}
