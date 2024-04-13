static bool isViewportConstrainedFixedOrStickyLayer(const RenderLayer* layer)
{
    if (layer->renderer()->isStickyPositioned())
        return !layer->enclosingOverflowClipLayer(ExcludeSelf);

    if (layer->renderer()->style()->position() != FixedPosition)
        return false;

    for (const RenderLayerStackingNode* stackingContainer = layer->stackingNode(); stackingContainer;
        stackingContainer = stackingContainer->ancestorStackingContainerNode()) {
        if (stackingContainer->layer()->compositingState() != NotComposited
            && stackingContainer->layer()->renderer()->style()->position() == FixedPosition)
            return false;
    }

    return true;
}
