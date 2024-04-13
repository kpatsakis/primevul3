void RenderLayerCompositor::repaintCompositedLayers(const IntRect* absRect)
{
    recursiveRepaintLayer(rootRenderLayer(), absRect);
}
