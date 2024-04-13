void RenderLayerCompositor::assignLayersToBackingsForReflectionLayer(RenderLayer* reflectionLayer, bool& layersChanged)
{
    if (computeCompositedLayerUpdate(reflectionLayer) != NoCompositingStateChange) {
        layersChanged = true;
        allocateOrClearCompositedLayerMapping(reflectionLayer);
    }
    updateDirectCompositingReasons(reflectionLayer);
    if (reflectionLayer->hasCompositedLayerMapping())
        reflectionLayer->compositedLayerMapping()->updateGraphicsLayerConfiguration();
}
