int RenderBox::flipForWritingMode(int position) const
{
    if (!style()->isFlippedBlocksWritingMode())
        return position;
    return logicalHeight() - position;
}
