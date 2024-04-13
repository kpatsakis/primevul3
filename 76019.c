void RenderBox::paintMaskImages(const PaintInfo& paintInfo, const LayoutRect& paintRect)
{
    bool pushTransparencyLayer = false;
    bool compositedMask = hasLayer() && layer()->hasCompositedMask();
    CompositeOperator compositeOp = CompositeSourceOver;

    bool allMaskImagesLoaded = true;
    
    if (!compositedMask) {
        const AffineTransform& currentCTM = paintInfo.context->getCTM();
        pushTransparencyLayer = !currentCTM.isIdentityOrTranslationOrFlipped();

        StyleImage* maskBoxImage = style()->maskBoxImage().image();
        const FillLayer* maskLayers = style()->maskLayers();

        if (maskBoxImage)
            allMaskImagesLoaded &= maskBoxImage->isLoaded();

        if (maskLayers)
            allMaskImagesLoaded &= maskLayers->imagesAreLoaded();

        if (!allMaskImagesLoaded)
            pushTransparencyLayer = true;

        if (maskBoxImage && maskLayers->hasImage()) {
            pushTransparencyLayer = true;
        } else {
            for (const FillLayer* fillLayer = maskLayers->next(); fillLayer; fillLayer = fillLayer->next()) {
                if (fillLayer->hasImage() && fillLayer->image()->canRender(style()->effectiveZoom())) {
                    pushTransparencyLayer = true;
                    break;
                }
            }
        }
        
        compositeOp = CompositeDestinationIn;
        if (pushTransparencyLayer) {
            paintInfo.context->setCompositeOperation(CompositeDestinationIn);
            paintInfo.context->beginTransparencyLayer(1.0f);
            compositeOp = CompositeSourceOver;
        }
    }

    if (allMaskImagesLoaded) {
        paintFillLayers(paintInfo, Color(), style()->maskLayers(), paintRect, BackgroundBleedNone, compositeOp);
        paintNinePieceImage(paintInfo.context, paintRect, style(), style()->maskBoxImage(), compositeOp);
    }
    
    if (pushTransparencyLayer)
        paintInfo.context->endTransparencyLayer();
}
