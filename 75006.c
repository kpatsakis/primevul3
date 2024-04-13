void InlineFlowBox::extractLineBoxFromRenderObject()
{
    toRenderInline(renderer())->lineBoxes()->extractLineBox(this);
}
