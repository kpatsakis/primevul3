void RenderBox::paintBoxDecorations(PaintInfo& paintInfo, const LayoutPoint& paintOffset)
{
    if (!paintInfo.shouldPaintWithinRoot(this))
        return;
    LayoutRect paintRect(paintOffset, size());

    borderFitAdjust(paintRect);

    paintBoxShadow(paintInfo.context, paintRect, style(), Normal);

    BackgroundBleedAvoidance bleedAvoidance = determineBackgroundBleedAvoidance(paintInfo.context);

    GraphicsContextStateSaver stateSaver(*paintInfo.context, false);
    if (bleedAvoidance == BackgroundBleedUseTransparencyLayer) {
        RoundedRect border = style()->getRoundedBorderFor(paintRect);
        stateSaver.save();
        paintInfo.context->addRoundedRectClip(border);
        paintInfo.context->beginTransparencyLayer(1);
    }
    
    bool themePainted = style()->hasAppearance() && !theme()->paint(this, paintInfo, paintRect);
    if (!themePainted) {
        if (isRoot())
            paintRootBoxFillLayers(paintInfo);
        else if (!isBody() || document()->documentElement()->renderer()->hasBackground()) {
            paintFillLayers(paintInfo, style()->visitedDependentColor(CSSPropertyBackgroundColor), style()->backgroundLayers(), paintRect, bleedAvoidance);
        }
        if (style()->hasAppearance())
            theme()->paintDecorations(this, paintInfo, paintRect);
    }
    paintBoxShadow(paintInfo.context, paintRect, style(), Inset);

    if ((!style()->hasAppearance() || (!themePainted && theme()->paintBorderOnly(this, paintInfo, paintRect))) && style()->hasBorder())
        paintBorder(paintInfo, paintRect, style(), bleedAvoidance);

    if (bleedAvoidance == BackgroundBleedUseTransparencyLayer)
        paintInfo.context->endTransparencyLayer();
}
