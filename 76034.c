int RenderBox::scrollLeft() const
{
    return hasOverflowClip() ? layer()->scrollXOffset() : 0;
}
