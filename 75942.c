void RenderBox::addOverflowFromChild(RenderBox* child, const LayoutSize& delta)
{
    LayoutRect childLayoutOverflowRect = child->layoutOverflowRectForPropagation(style());
    childLayoutOverflowRect.move(delta);
    addLayoutOverflow(childLayoutOverflowRect);
            
    if (child->hasSelfPaintingLayer() || hasOverflowClip())
        return;
    LayoutRect childVisualOverflowRect = child->visualOverflowRectForPropagation(style());
    childVisualOverflowRect.move(delta);
    addVisualOverflow(childVisualOverflowRect);
}
