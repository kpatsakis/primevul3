bool RenderBox::pushContentsClip(PaintInfo& paintInfo, const LayoutPoint& accumulatedOffset)
{
    if (paintInfo.phase == PaintPhaseBlockBackground || paintInfo.phase == PaintPhaseSelfOutline || paintInfo.phase == PaintPhaseMask)
        return false;
        
    bool isControlClip = hasControlClip();
    bool isOverflowClip = hasOverflowClip() && !layer()->isSelfPaintingLayer();
    
    if (!isControlClip && !isOverflowClip)
        return false;
    
    if (paintInfo.phase == PaintPhaseOutline)
        paintInfo.phase = PaintPhaseChildOutlines;
    else if (paintInfo.phase == PaintPhaseChildBlockBackground) {
        paintInfo.phase = PaintPhaseBlockBackground;
        paintObject(paintInfo, accumulatedOffset);
        paintInfo.phase = PaintPhaseChildBlockBackgrounds;
    }
    IntRect clipRect(isControlClip ? controlClipRect(accumulatedOffset) : overflowClipRect(accumulatedOffset));
    paintInfo.context->save();
    if (style()->hasBorderRadius())
        paintInfo.context->addRoundedRectClip(style()->getRoundedBorderFor(IntRect(accumulatedOffset, size())));
    paintInfo.context->clip(clipRect);
    return true;
}
