LayoutUnit RenderBox::marginBefore() const
{
    switch (style()->writingMode()) {
    case TopToBottomWritingMode:
        return m_marginTop;
    case BottomToTopWritingMode:
        return m_marginBottom;
    case LeftToRightWritingMode:
        return m_marginLeft;
    case RightToLeftWritingMode:
        return m_marginRight;
    }
    ASSERT_NOT_REACHED();
    return m_marginTop;
}
