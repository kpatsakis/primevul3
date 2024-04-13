void RenderBox::addVisualOverflow(const LayoutRect& rect)
{
    LayoutRect borderBox = borderBoxRect();
    if (borderBox.contains(rect) || rect.isEmpty())
        return;
        
    if (!m_overflow)
        m_overflow = adoptPtr(new RenderOverflow(clientBoxRect(), borderBox));
    
    m_overflow->addVisualOverflow(rect);
}
