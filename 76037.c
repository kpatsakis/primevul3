void RenderBox::setMarginAfter(LayoutUnit margin)
{
    switch (style()->writingMode()) {
    case TopToBottomWritingMode:
        m_marginBottom = margin;
        break;
    case BottomToTopWritingMode:
        m_marginTop = margin;
        break;
    case LeftToRightWritingMode:
        m_marginRight = margin;
        break;
    case RightToLeftWritingMode:
        m_marginLeft = margin;
        break;
    }
}
