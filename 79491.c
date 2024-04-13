RenderLayerCompositor::CompositingStateTransitionType RenderLayerCompositor::computeCompositedLayerUpdate(const RenderLayer* layer)
{
    CompositingStateTransitionType update = NoCompositingStateChange;
    if (needsOwnBacking(layer)) {
        if (!layer->hasCompositedLayerMapping()) {
            update = AllocateOwnCompositedLayerMapping;
        }
    } else {
        if (layer->hasCompositedLayerMapping())
            update = RemoveOwnCompositedLayerMapping;

        if (layerSquashingEnabled()) {
            if (requiresSquashing(layer->compositingReasons())) {
                update = AddToSquashingLayer;
            } else if (layer->groupedMapping()) {
                update = RemoveFromSquashingLayer;
            }
        }
    }
    return update;
}
