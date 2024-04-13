void InlineFlowBox::addReplacedChildOverflow(const InlineBox* inlineBox, IntRect& logicalLayoutOverflow, IntRect& logicalVisualOverflow)
{
    RenderBox* box = toRenderBox(inlineBox->renderer());
    
    if (!box->hasSelfPaintingLayer()) {
        IntRect childLogicalVisualOverflow = box->logicalVisualOverflowRectForPropagation(renderer()->style());
        childLogicalVisualOverflow.move(inlineBox->logicalLeft(), inlineBox->logicalTop());
        logicalVisualOverflow.unite(childLogicalVisualOverflow);
    }

    IntRect childLogicalLayoutOverflow = box->logicalLayoutOverflowRectForPropagation(renderer()->style());
    childLogicalLayoutOverflow.move(inlineBox->logicalLeft(), inlineBox->logicalTop());
    logicalLayoutOverflow.unite(childLogicalLayoutOverflow);
}
