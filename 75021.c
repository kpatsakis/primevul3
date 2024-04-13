void InlineFlowBox::removeLineBoxFromRenderObject()
{
    toRenderInline(renderer())->lineBoxes()->removeLineBox(this);
}
