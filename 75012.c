void InlineFlowBox::paintBoxDecorations(PaintInfo& paintInfo, int tx, int ty)
{
    if (!paintInfo.shouldPaintWithinRoot(renderer()) || renderer()->style()->visibility() != VISIBLE || paintInfo.phase != PaintPhaseForeground)
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
    
    GraphicsContext* context = paintInfo.context;
    
    RenderStyle* styleToUse = renderer()->style(m_firstLine);
    if ((!parent() && m_firstLine && styleToUse != renderer()->style()) || (parent() && renderer()->hasBoxDecorations())) {
        paintBoxShadow(context, styleToUse, Normal, tx, ty, w, h);

        Color c = styleToUse->visitedDependentColor(CSSPropertyBackgroundColor);
        paintFillLayers(paintInfo, c, styleToUse->backgroundLayers(), tx, ty, w, h);
        paintBoxShadow(context, styleToUse, Inset, tx, ty, w, h);

        if (parent() && renderer()->style()->hasBorder()) {
            StyleImage* borderImage = renderer()->style()->borderImage().image();
            bool hasBorderImage = borderImage && borderImage->canRender(styleToUse->effectiveZoom());
            if (hasBorderImage && !borderImage->isLoaded())
                return; // Don't paint anything while we wait for the image to load.

            if (!hasBorderImage || (!prevLineBox() && !nextLineBox()))
                boxModelObject()->paintBorder(context, tx, ty, w, h, renderer()->style(), includeLogicalLeftEdge(), includeLogicalRightEdge());
            else {
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
                context->save();
                context->clip(IntRect(tx, ty, w, h));
                boxModelObject()->paintBorder(context, stripX, stripY, stripWidth, stripHeight, renderer()->style());
                context->restore();
            }
        }
    }
}
