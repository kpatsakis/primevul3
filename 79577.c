void RenderLayerCompositor::updateLayerCompositingState(RenderLayer* layer)
{
    updateDirectCompositingReasons(layer);
    CompositingStateTransitionType compositedLayerUpdate = computeCompositedLayerUpdate(layer);

    if (compositedLayerUpdate != NoCompositingStateChange) {
        setCompositingLayersNeedRebuild();
        setNeedsToRecomputeCompositingRequirements();
    }

    if (layer->hasCompositedLayerMapping() && layer->compositedLayerMapping()->updateGraphicsLayerConfiguration()) {
        setCompositingLayersNeedRebuild();
    } else if (compositedLayerUpdate == NoCompositingStateChange) {
        if (layer->compositingState() == PaintsIntoOwnBacking || layer->compositingState() == HasOwnBackingButPaintsIntoAncestor)
            setCompositingLayersNeedRebuild();
    }

    applyUpdateLayerCompositingStateChickenEggHacks(layer, compositedLayerUpdate);
}
