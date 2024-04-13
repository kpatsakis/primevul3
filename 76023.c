void RenderBox::popContentsClip(PaintInfo& paintInfo, PaintPhase originalPhase, const LayoutPoint& accumulatedOffset)
{
    ASSERT(hasControlClip() || (hasOverflowClip() && !layer()->isSelfPaintingLayer()));

    paintInfo.context->restore();
    if (originalPhase == PaintPhaseOutline) {
        paintInfo.phase = PaintPhaseSelfOutline;
        paintObject(paintInfo, accumulatedOffset);
        paintInfo.phase = originalPhase;
    } else if (originalPhase == PaintPhaseChildBlockBackground)
        paintInfo.phase = originalPhase;
}
