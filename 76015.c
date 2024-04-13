void RenderBox::paint(PaintInfo& paintInfo, const LayoutPoint& paintOffset)
{
    LayoutPoint adjustedPaintOffset = paintOffset + location();
    PaintInfo childInfo(paintInfo);
    childInfo.updatePaintingRootForChildren(this);
    for (RenderObject* child = firstChild(); child; child = child->nextSibling())
        child->paint(childInfo, adjustedPaintOffset);
}
