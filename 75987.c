FloatPoint RenderBox::flipForWritingMode(const FloatPoint& position) const
{
    if (!style()->isFlippedBlocksWritingMode())
        return position;
    return isHorizontalWritingMode() ? FloatPoint(position.x(), height() - position.y()) : FloatPoint(width() - position.x(), position.y());
}
