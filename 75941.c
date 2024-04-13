void RenderBox::addLayoutOverflow(const LayoutRect& rect)
{
    LayoutRect clientBox = clientBoxRect();
    if (clientBox.contains(rect) || rect.isEmpty())
        return;
    
    LayoutRect overflowRect(rect);
    if (hasOverflowClip() || isRenderView()) {
        bool hasTopOverflow = !style()->isLeftToRightDirection() && !isHorizontalWritingMode();
        bool hasLeftOverflow = !style()->isLeftToRightDirection() && isHorizontalWritingMode();
        
        if (!hasTopOverflow)
            overflowRect.shiftYEdgeTo(max(overflowRect.y(), clientBox.y()));
        else
            overflowRect.shiftMaxYEdgeTo(min(overflowRect.maxY(), clientBox.maxY()));
        if (!hasLeftOverflow)
            overflowRect.shiftXEdgeTo(max(overflowRect.x(), clientBox.x()));
        else
            overflowRect.shiftMaxXEdgeTo(min(overflowRect.maxX(), clientBox.maxX()));
        
        if (clientBox.contains(overflowRect) || overflowRect.isEmpty())
            return;
    }

    if (!m_overflow)
        m_overflow = adoptPtr(new RenderOverflow(clientBox, borderBoxRect()));
    
    m_overflow->addLayoutOverflow(overflowRect);
}
