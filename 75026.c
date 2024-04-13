void InlineFlowBox::setLayoutOverflow(const IntRect& rect)
{
    IntRect frameBox = enclosingIntRect(FloatRect(x(), y(), width(), height()));
    if (frameBox == rect || rect.isEmpty())
        return;
        
    if (!m_overflow)
        m_overflow.set(new RenderOverflow(frameBox, frameBox));
    
    m_overflow->setLayoutOverflow(rect);
}
