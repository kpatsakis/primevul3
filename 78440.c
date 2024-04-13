static int computeOffset(WebCore::RenderObject* renderer, int x, int y)
{
    return WebCore::VisiblePosition(renderer->positionForPoint(WebCore::LayoutPoint(x, y))).deepEquivalent().computeOffsetInContainerNode();
}
