void InlineFlowBox::paintMask(PaintInfo& paintInfo, int tx, int ty)
{
    if (!paintInfo.shouldPaintWithinRoot(renderer()) || renderer()->style()->visibility() != VISIBLE || paintInfo.phase != PaintPhaseMask)
        return;

    IntRect frameRect = roundedFrameRect();
    int x = frameRect.x();
    int y = frameRect.y();
    int w = frameRect.width();
    int h = frameRect.height();

    bool noQuirksMode = renderer()->document()->inNoQuirksMode();
    if (!hasTextChildren() && !noQuirksMode) {
        RootInlineBox* rootBox = root();
        int& top = isHorizontal() ? y : x;
        int& logicalHeight = isHorizontal() ? h : w;
        int bottom = min(rootBox->lineBottom(), top + logicalHeight);
        top = max(rootBox->lineTop(), top);
        logicalHeight = bottom - top;
    }
    
    IntRect localRect(x, y, w, h);
    flipForWritingMode(localRect);
    tx += localRect.x();
    ty += localRect.y();

    const NinePieceImage& maskNinePieceImage = renderer()->style()->maskBoxImage();
    StyleImage* maskBoxImage = renderer()->style()->maskBoxImage().image();

    bool pushTransparencyLayer = false;
    bool compositedMask = renderer()->hasLayer() && boxModelObject()->layer()->hasCompositedMask();
    CompositeOperator compositeOp = CompositeSourceOver;
    if (!compositedMask) {
        if ((maskBoxImage && renderer()->style()->maskLayers()->hasImage()) || renderer()->style()->maskLayers()->next())
            pushTransparencyLayer = true;
        
        compositeOp = CompositeDestinationIn;
        if (pushTransparencyLayer) {
            paintInfo.context->setCompositeOperation(CompositeDestinationIn);
            paintInfo.context->beginTransparencyLayer(1.0f);
            compositeOp = CompositeSourceOver;
        }
    }

    paintFillLayers(paintInfo, Color(), renderer()->style()->maskLayers(), tx, ty, w, h, compositeOp);
    
    bool hasBoxImage = maskBoxImage && maskBoxImage->canRender(renderer()->style()->effectiveZoom());
    if (!hasBoxImage || !maskBoxImage->isLoaded())
        return; // Don't paint anything while we wait for the image to load.

    if (!prevLineBox() && !nextLineBox()) {
        boxModelObject()->paintNinePieceImage(paintInfo.context, tx, ty, w, h, renderer()->style(), maskNinePieceImage, compositeOp);
    } else {
        int logicalOffsetOnLine = 0;
        for (InlineFlowBox* curr = prevLineBox(); curr; curr = curr->prevLineBox())
            logicalOffsetOnLine += curr->logicalWidth();
        int totalLogicalWidth = logicalOffsetOnLine;
        for (InlineFlowBox* curr = this; curr; curr = curr->nextLineBox())
            totalLogicalWidth += curr->logicalWidth();
        int stripX = tx - (isHorizontal() ? logicalOffsetOnLine : 0);
        int stripY = ty - (isHorizontal() ? 0 : logicalOffsetOnLine);
        int stripWidth = isHorizontal() ? totalLogicalWidth : w;
        int stripHeight = isHorizontal() ? h : totalLogicalWidth;
        paintInfo.context->save();
        paintInfo.context->clip(IntRect(tx, ty, w, h));
        boxModelObject()->paintNinePieceImage(paintInfo.context, stripX, stripY, stripWidth, stripHeight, renderer()->style(), maskNinePieceImage, compositeOp);
        paintInfo.context->restore();
    }
    
    if (pushTransparencyLayer)
        paintInfo.context->endTransparencyLayer();
}
