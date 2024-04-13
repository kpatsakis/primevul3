void GraphicsContext::setPlatformShadow(const FloatSize& size,
                                        float blurFloat,
                                        const Color& color,
                                        ColorSpace colorSpace)
{
    if (paintingDisabled())
        return;

    if (!size.width() && !size.height() && !blurFloat) {
        platformContext()->setDrawLooper(0);
        return;
    }

    double width = size.width();
    double height = size.height();
    double blur = blurFloat;

    uint32_t mfFlags = SkBlurMaskFilter::kHighQuality_BlurFlag;
    SkXfermode::Mode colorMode = SkXfermode::kSrc_Mode;

    if (m_state.shadowsIgnoreTransforms)  {
        mfFlags |= SkBlurMaskFilter::kIgnoreTransform_BlurFlag;

        colorMode = SkXfermode::kDst_Mode;

        height = -height;
    }

    SkColor c;
    if (color.isValid())
        c = color.rgb();
    else
        c = SkColorSetARGB(0xFF/3, 0, 0, 0);    // "std" apple shadow color.


    SkLayerDrawLooper* dl = new SkLayerDrawLooper;
    SkAutoUnref aur(dl);

    dl->addLayer();

    SkLayerDrawLooper::LayerInfo info;

    info.fPaintBits |= SkLayerDrawLooper::kMaskFilter_Bit; // our blur
    info.fPaintBits |= SkLayerDrawLooper::kColorFilter_Bit;
    info.fColorMode = colorMode;
    info.fOffset.set(width, height);
    info.fPostTranslate = m_state.shadowsIgnoreTransforms;

    SkMaskFilter* mf = SkBlurMaskFilter::Create(blur / 2, SkBlurMaskFilter::kNormal_BlurStyle, mfFlags);

    SkColorFilter* cf = SkColorFilter::CreateModeFilter(c, SkXfermode::kSrcIn_Mode);

    SkPaint* paint = dl->addLayer(info);
    SkSafeUnref(paint->setMaskFilter(mf));
    SkSafeUnref(paint->setColorFilter(cf));

    platformContext()->setDrawLooper(dl);
}
