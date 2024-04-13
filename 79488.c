void RenderLayerCompositor::clearMappingForRenderLayerIncludingDescendants(RenderLayer* layer)
{
    if (!layer)
        return;

    if (layer->hasCompositedLayerMapping()) {
        removeViewportConstrainedLayer(layer);
        layer->clearCompositedLayerMapping();
    }

    for (RenderLayer* currLayer = layer->firstChild(); currLayer; currLayer = currLayer->nextSibling())
        clearMappingForRenderLayerIncludingDescendants(currLayer);
}
