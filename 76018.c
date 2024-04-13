void RenderBox::paintMask(PaintInfo& paintInfo, const LayoutPoint& paintOffset)
{
    if (!paintInfo.shouldPaintWithinRoot(this) || style()->visibility() != VISIBLE || paintInfo.phase != PaintPhaseMask || paintInfo.context->paintingDisabled())
        return;

    LayoutRect paintRect = LayoutRect(paintOffset, size());

    borderFitAdjust(paintRect);

    paintMaskImages(paintInfo, paintRect);
}
