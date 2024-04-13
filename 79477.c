bool RenderLayerCompositor::allocateOrClearCompositedLayerMapping(RenderLayer* layer)
{
    bool compositedLayerMappingChanged = false;
    bool nonCompositedReasonChanged = updateLayerIfViewportConstrained(layer);
    CompositingStateTransitionType compositedLayerUpdate = computeCompositedLayerUpdate(layer);

    switch (compositedLayerUpdate) {
    case AllocateOwnCompositedLayerMapping:
        ASSERT(!layer->hasCompositedLayerMapping());
        enableCompositingMode();

        repaintOnCompositingChange(layer);
        layer->ensureCompositedLayerMapping();
        compositedLayerMappingChanged = true;

        if (layer->isRootLayer() && isMainFrame()) {
            if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
                scrollingCoordinator->frameViewRootLayerDidChange(m_renderView->frameView());
        }

        layer->setGroupedMapping(0);

        if (layer->parent())
            layer->repainter().computeRepaintRectsIncludingDescendants();

        break;
    case RemoveOwnCompositedLayerMapping:
    case AddToSquashingLayer:
        if (layer->hasCompositedLayerMapping()) {
            if (layer->isReflection()) {
                RenderLayer* sourceLayer = toRenderLayerModelObject(layer->renderer()->parent())->layer();
                if (sourceLayer->hasCompositedLayerMapping()) {
                    ASSERT(sourceLayer->compositedLayerMapping()->mainGraphicsLayer()->replicaLayer() == layer->compositedLayerMapping()->mainGraphicsLayer());
                    sourceLayer->compositedLayerMapping()->mainGraphicsLayer()->setReplicatedByLayer(0);
                }
            }

            removeViewportConstrainedLayer(layer);

            layer->clearCompositedLayerMapping();
            compositedLayerMappingChanged = true;

            layer->repainter().computeRepaintRectsIncludingDescendants();

            repaintOnCompositingChange(layer);
        }

        break;
    case RemoveFromSquashingLayer:
    case NoCompositingStateChange:
        break;
    }

    if (layer->hasCompositedLayerMapping() && layer->compositedLayerMapping()->updateRequiresOwnBackingStoreForIntrinsicReasons())
        compositedLayerMappingChanged = true;

    if (compositedLayerMappingChanged && layer->renderer()->isRenderPart()) {
        RenderLayerCompositor* innerCompositor = frameContentsCompositor(toRenderPart(layer->renderer()));
        if (innerCompositor && innerCompositor->inCompositingMode())
            innerCompositor->updateRootLayerAttachment();
    }

    if (compositedLayerMappingChanged)
        layer->clipper().clearClipRectsIncludingDescendants(PaintingClipRects);

    if (compositedLayerMappingChanged || nonCompositedReasonChanged) {
        if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
            scrollingCoordinator->frameViewFixedObjectsDidChange(m_renderView->frameView());
    }

    return compositedLayerMappingChanged || nonCompositedReasonChanged;
}
