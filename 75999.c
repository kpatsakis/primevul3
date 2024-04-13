IntRect RenderBox::logicalVisualOverflowRectForPropagation(RenderStyle* parentStyle) const
{
    IntRect rect = visualOverflowRectForPropagation(parentStyle);
    if (!parentStyle->isHorizontalWritingMode())
        return rect.transposedRect();
    return rect;
}
