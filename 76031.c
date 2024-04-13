void RenderBox::repaintDuringLayoutIfMoved(const LayoutRect& rect)
{
    LayoutUnit newX = x();
    LayoutUnit newY = y();
    LayoutUnit newWidth = width();
    LayoutUnit newHeight = height();
    if (rect.x() != newX || rect.y() != newY) {
        m_frameRect = rect;
        repaint();
        repaintOverhangingFloats(true);
        m_frameRect = LayoutRect(newX, newY, newWidth, newHeight);
        repaint();
        repaintOverhangingFloats(true);
    }
}
