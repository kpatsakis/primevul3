bool RenderLayerCompositor::layerSquashingEnabled() const
{
    if (Settings* settings = m_renderView->document().settings())
        return settings->layerSquashingEnabled();

    return false;
}
