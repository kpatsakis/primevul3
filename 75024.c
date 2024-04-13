IntRect InlineFlowBox::roundedFrameRect() const
{
    int snappedX = lroundf(x());
    int snappedY = lroundf(y());
    
    int snappedMaxX = lroundf(x() + width());
    int snappedMaxY = lroundf(y() + height());
    
    return IntRect(snappedX, snappedY, snappedMaxX - snappedX, snappedMaxY - snappedY);
}
