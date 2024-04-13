LayoutPoint RenderBox::flipForWritingMode(const RenderBox* child, const LayoutPoint& point, FlippingAdjustment adjustment) const
{
    if (!style()->isFlippedBlocksWritingMode())
        return point;
    
    if (isHorizontalWritingMode())
        return LayoutPoint(point.x(), point.y() + height() - child->height() - child->y() - (adjustment == ParentToChildFlippingAdjustment ? child->y() : 0));
    return LayoutPoint(point.x() + width() - child->width() - child->x() - (adjustment == ParentToChildFlippingAdjustment ? child->x() : 0), point.y());
}
