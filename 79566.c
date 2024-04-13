bool RenderLayerCompositor::scrollingLayerDidChange(RenderLayer* layer)
{
    if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
        return scrollingCoordinator->scrollableAreaScrollLayerDidChange(layer->scrollableArea());
    return false;
}
