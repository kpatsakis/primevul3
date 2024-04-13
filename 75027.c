void InlineFlowBox::setOverflowFromLogicalRects(const IntRect& logicalLayoutOverflow, const IntRect& logicalVisualOverflow)
{
    IntRect layoutOverflow(isHorizontal() ? logicalLayoutOverflow : logicalLayoutOverflow.transposedRect());
    setLayoutOverflow(layoutOverflow);
    
    IntRect visualOverflow(isHorizontal() ? logicalVisualOverflow : logicalVisualOverflow.transposedRect());
    setVisualOverflow(visualOverflow);
}
