void RenderBox::addFocusRingRects(Vector<LayoutRect>& rects, const LayoutPoint& additionalOffset)
{
    if (!size().isEmpty())
        rects.append(LayoutRect(additionalOffset, size()));
}
