void RenderBox::setMarginBefore(LayoutUnit margin)
{
    switch (style()->writingMode()) {
    case TopToBottomWritingMode:
        m_marginTop = margin;
        break;
    case BottomToTopWritingMode:
        m_marginBottom = margin;
        break;
    case LeftToRightWritingMode:
        m_marginLeft = margin;
        break;
    case RightToLeftWritingMode:
        m_marginRight = margin;
        break;
    }
}
