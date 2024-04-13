RenderLineBoxList* InlineFlowBox::rendererLineBoxes() const
{
    return toRenderInline(renderer())->lineBoxes();
}
