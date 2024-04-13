void InlineFlowBox::attachLineBoxToRenderObject()
{
    toRenderInline(renderer())->lineBoxes()->attachLineBox(this);
}
