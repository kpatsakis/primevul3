void RenderBox::setMarginStart(LayoutUnit margin)
{
    if (isHorizontalWritingMode()) {
        if (style()->isLeftToRightDirection())
            m_marginLeft = margin;
        else
            m_marginRight = margin;
    } else {
        if (style()->isLeftToRightDirection())
            m_marginTop = margin;
        else
            m_marginBottom = margin;
    }
}
