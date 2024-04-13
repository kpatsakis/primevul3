int InlineTextBox::placeEllipsisBox(bool flowIsLTR, int visibleLeftEdge, int visibleRightEdge, int ellipsisWidth, bool& foundBox)
{
    if (foundBox) {
        m_truncation = cFullTruncation;
        return -1;
    }

    int ellipsisX = flowIsLTR ? visibleRightEdge - ellipsisWidth : visibleLeftEdge + ellipsisWidth;
    
    bool ltrFullTruncation = flowIsLTR && ellipsisX <= m_x;
    bool rtlFullTruncation = !flowIsLTR && ellipsisX >= (m_x + m_logicalWidth);
    if (ltrFullTruncation || rtlFullTruncation) {
        m_truncation = cFullTruncation;
        foundBox = true;
        return -1;
    }

    bool ltrEllipsisWithinBox = flowIsLTR && (ellipsisX < m_x + m_logicalWidth);
    bool rtlEllipsisWithinBox = !flowIsLTR && (ellipsisX > m_x);
    if (ltrEllipsisWithinBox || rtlEllipsisWithinBox) {
        foundBox = true;

        bool ltr = isLeftToRightDirection();
        if (ltr != flowIsLTR) {
          int visibleBoxWidth = visibleRightEdge - visibleLeftEdge  - ellipsisWidth;
          ellipsisX = ltr ? m_x + visibleBoxWidth : m_x + m_logicalWidth - visibleBoxWidth;
        }

        int offset = offsetForPosition(ellipsisX, false);
        if (offset == 0) {
            m_truncation = cFullTruncation;
            return min(ellipsisX, m_x);
        }

        m_truncation = offset;

        int widthOfVisibleText = toRenderText(renderer())->width(m_start, offset, textPos(), m_firstLine);

        if (flowIsLTR)
            return m_x + widthOfVisibleText;
        else
            return (m_x + m_logicalWidth) - widthOfVisibleText - ellipsisWidth;
    }
    return -1;
}
