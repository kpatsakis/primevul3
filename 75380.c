LayoutRect RenderMenuList::controlClipRect(const LayoutPoint& additionalOffset) const
{
    LayoutRect outerBox(additionalOffset.x() + borderLeft() + paddingLeft(), 
                   additionalOffset.y() + borderTop() + paddingTop(),
                   contentWidth(), 
                   contentHeight());
    
    LayoutRect innerBox(additionalOffset.x() + m_innerBlock->x() + m_innerBlock->paddingLeft(), 
                   additionalOffset.y() + m_innerBlock->y() + m_innerBlock->paddingTop(),
                   m_innerBlock->contentWidth(), 
                   m_innerBlock->contentHeight());

    return intersection(outerBox, innerBox);
}
