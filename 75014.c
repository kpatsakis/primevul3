void InlineFlowBox::paintFillLayer(const PaintInfo& paintInfo, const Color& c, const FillLayer* fillLayer, int tx, int ty, int w, int h, CompositeOperator op)
{
    StyleImage* img = fillLayer->image();
    bool hasFillImage = img && img->canRender(renderer()->style()->effectiveZoom());
    if ((!hasFillImage && !renderer()->style()->hasBorderRadius()) || (!prevLineBox() && !nextLineBox()) || !parent())
        boxModelObject()->paintFillLayerExtended(paintInfo, c, fillLayer, tx, ty, w, h, this, op);
    else {
        int logicalOffsetOnLine = 0;
        int totalLogicalWidth;
        if (renderer()->style()->direction() == LTR) {
            for (InlineFlowBox* curr = prevLineBox(); curr; curr = curr->prevLineBox())
                logicalOffsetOnLine += curr->logicalWidth();
            totalLogicalWidth = logicalOffsetOnLine;
            for (InlineFlowBox* curr = this; curr; curr = curr->nextLineBox())
                totalLogicalWidth += curr->logicalWidth();
        } else {
            for (InlineFlowBox* curr = nextLineBox(); curr; curr = curr->nextLineBox())
                logicalOffsetOnLine += curr->logicalWidth();
            totalLogicalWidth = logicalOffsetOnLine;
            for (InlineFlowBox* curr = this; curr; curr = curr->prevLineBox())
                totalLogicalWidth += curr->logicalWidth();
        }
        int stripX = tx - (isHorizontal() ? logicalOffsetOnLine : 0);
        int stripY = ty - (isHorizontal() ? 0 : logicalOffsetOnLine);
        int stripWidth = isHorizontal() ? totalLogicalWidth : width();
        int stripHeight = isHorizontal() ? height() : totalLogicalWidth;
        paintInfo.context->save();
        paintInfo.context->clip(IntRect(tx, ty, width(), height()));
        boxModelObject()->paintFillLayerExtended(paintInfo, c, fillLayer, stripX, stripY, stripWidth, stripHeight, this, op);
        paintInfo.context->restore();
    }
}
