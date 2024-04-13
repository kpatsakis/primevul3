LayoutUnit RenderBox::marginAfter() const
{
    switch (style()->writingMode()) {
    case TopToBottomWritingMode:
        return m_marginBottom;
    case BottomToTopWritingMode:
        return m_marginTop;
    case LeftToRightWritingMode:
        return m_marginRight;
    case RightToLeftWritingMode:
        return m_marginLeft;
    }
    ASSERT_NOT_REACHED();
    return m_marginBottom;
}
