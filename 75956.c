static void computeBlockStaticDistance(Length& logicalTop, Length& logicalBottom, const RenderBox* child, const RenderBoxModelObject* containerBlock)
{
    if (!logicalTop.isAuto() || !logicalBottom.isAuto())
        return;
    
    int staticLogicalTop = child->layer()->staticBlockPosition() - containerBlock->borderBefore();
    for (RenderObject* curr = child->parent(); curr && curr != containerBlock; curr = curr->container()) {
        if (curr->isBox() && !curr->isTableRow())
            staticLogicalTop += toRenderBox(curr)->logicalTop();
    }
    logicalTop.setValue(Fixed, staticLogicalTop);
}
