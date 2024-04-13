void RenderBox::setScrollLeft(int newLeft)
{
    if (hasOverflowClip())
        layer()->scrollToXOffset(newLeft, RenderLayer::ScrollOffsetClamped);
}
