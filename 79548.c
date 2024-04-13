bool RenderLayerCompositor::requiresCompositingForPlugin(RenderObject* renderer) const
{
    if (!(m_compositingTriggers & ChromeClient::PluginTrigger))
        return false;

    bool composite = renderer->isEmbeddedObject() && toRenderEmbeddedObject(renderer)->allowsAcceleratedCompositing();
    if (!composite)
        return false;

    m_needsToRecomputeCompositingRequirements = true;

    RenderWidget* pluginRenderer = toRenderWidget(renderer);
    if (pluginRenderer->needsLayout())
        return pluginRenderer->hasLayer() && pluginRenderer->layer()->hasCompositedLayerMapping();

    IntRect contentBox = pixelSnappedIntRect(pluginRenderer->contentBoxRect());
    return contentBox.height() * contentBox.width() > 1;
}
