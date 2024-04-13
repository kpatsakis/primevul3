bool RenderLayerCompositor::requiresCompositingForPosition(RenderObject* renderer, const RenderLayer* layer, RenderLayer::ViewportConstrainedNotCompositedReason* viewportConstrainedNotCompositedReason) const
{
    if (!renderer->isPositioned())
        return false;

    EPosition position = renderer->style()->position();
    bool isFixed = renderer->isOutOfFlowPositioned() && position == FixedPosition;
    if (isFixed && !layer->stackingNode()->isStackingContainer())
        return false;

    bool isSticky = renderer->isInFlowPositioned() && position == StickyPosition;
    if (!isFixed && !isSticky)
        return false;

    if (Settings* settings = m_renderView->document().settings()) {
        if (!settings->acceleratedCompositingForFixedPositionEnabled())
            return false;
    }

    if (isSticky)
        return isViewportConstrainedFixedOrStickyLayer(layer);

    RenderObject* container = renderer->container();
    if (!container) {
        m_needsToRecomputeCompositingRequirements = true;
        return false;
    }

    if (container != m_renderView) {
        if (viewportConstrainedNotCompositedReason)
            *viewportConstrainedNotCompositedReason = RenderLayer::NotCompositedForNonViewContainer;
        return false;
    }

    bool hasScrollableAncestor = false;

    FrameView* frameView = m_renderView->frameView();
    if (frameView && frameView->isScrollable())
        hasScrollableAncestor = true;

    RenderLayer* ancestor = layer->parent();
    while (ancestor && !hasScrollableAncestor) {
        if (frameView->containsScrollableArea(ancestor->scrollableArea()))
            hasScrollableAncestor = true;
        if (ancestor->renderer() == m_renderView)
            break;
        ancestor = ancestor->parent();
    }

    if (!hasScrollableAncestor) {
        if (viewportConstrainedNotCompositedReason)
            *viewportConstrainedNotCompositedReason = RenderLayer::NotCompositedForUnscrollableAncestors;
        return false;
    }

    if (!m_inPostLayoutUpdate) {
        m_needsToRecomputeCompositingRequirements = true;
        return layer->hasCompositedLayerMapping();
    }

    bool paintsContent = layer->isVisuallyNonEmpty() || layer->hasVisibleDescendant();
    if (!paintsContent) {
        if (viewportConstrainedNotCompositedReason)
            *viewportConstrainedNotCompositedReason = RenderLayer::NotCompositedForNoVisibleContent;
        return false;
    }

    if (FrameView* frameView = m_renderView->frameView()) {
        LayoutRect viewBounds = frameView->viewportConstrainedVisibleContentRect();
        LayoutRect layerBounds = layer->calculateLayerBounds(rootRenderLayer(), 0,
            RenderLayer::DefaultCalculateLayerBoundsFlags
            | RenderLayer::ExcludeHiddenDescendants
            | RenderLayer::DontConstrainForMask
            | RenderLayer::IncludeCompositedDescendants
            | RenderLayer::PretendLayerHasOwnBacking);
        if (!viewBounds.intersects(enclosingIntRect(layerBounds))) {
            if (viewportConstrainedNotCompositedReason) {
                *viewportConstrainedNotCompositedReason = RenderLayer::NotCompositedForBoundsOutOfView;
                m_needsToRecomputeCompositingRequirements = true;
            }
            return false;
        }
    }

    return true;
}
