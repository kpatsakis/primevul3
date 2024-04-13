void RenderLayerCompositor::removeViewportConstrainedLayer(RenderLayer* layer)
{
    if (!m_viewportConstrainedLayers.contains(layer))
        return;

    m_viewportConstrainedLayers.remove(layer);
}
