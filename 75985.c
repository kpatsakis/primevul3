IntPoint RenderBox::flipForWritingMode(const IntPoint& position) const
{
    if (!style()->isFlippedBlocksWritingMode())
        return position;
    return isHorizontalWritingMode() ? IntPoint(position.x(), height() - position.y()) : IntPoint(width() - position.x(), position.y());
}
