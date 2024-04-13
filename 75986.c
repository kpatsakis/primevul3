IntSize RenderBox::flipForWritingMode(const IntSize& offset) const
{
    if (!style()->isFlippedBlocksWritingMode())
        return offset;
    return isHorizontalWritingMode() ? IntSize(offset.width(), height() - offset.height()) : IntSize(width() - offset.width(), offset.height());
}
