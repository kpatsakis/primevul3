void GraphicsContext::beginPlatformTransparencyLayer(float opacity)
{
    if (paintingDisabled())
        return;


    SkPaint layerPaint;
    layerPaint.setAlpha(static_cast<unsigned char>(opacity * 255));
    layerPaint.setXfermodeMode(platformContext()->getXfermodeMode());

    platformContext()->canvas()->saveLayer(
        0,
        &layerPaint,
        static_cast<SkCanvas::SaveFlags>(SkCanvas::kHasAlphaLayer_SaveFlag |
                                         SkCanvas::kFullColorLayer_SaveFlag));
}
