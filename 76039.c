void RenderBox::setMarginEnd(LayoutUnit margin)
{
    if (isHorizontalWritingMode()) {
        if (style()->isLeftToRightDirection())
            m_marginRight = margin;
        else
            m_marginLeft = margin;
    } else {
        if (style()->isLeftToRightDirection())
            m_marginBottom = margin;
        else
            m_marginTop = margin;
    }
}
