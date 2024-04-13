void RenderBox::clearLayoutOverflow()
{
    if (!m_overflow)
        return;
    
    if (visualOverflowRect() == borderBoxRect()) {
        m_overflow.clear();
        return;
    }
    
    m_overflow->resetLayoutOverflow(borderBoxRect());
}
