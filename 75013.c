void InlineFlowBox::paintBoxShadow(GraphicsContext* context, RenderStyle* s, ShadowStyle shadowStyle, int tx, int ty, int w, int h)
{
    if ((!prevLineBox() && !nextLineBox()) || !parent())
        boxModelObject()->paintBoxShadow(context, tx, ty, w, h, s, shadowStyle);
    else {
        boxModelObject()->paintBoxShadow(context, tx, ty, w, h, s, shadowStyle, includeLogicalLeftEdge(), includeLogicalRightEdge());
    }
}
