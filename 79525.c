bool RenderLayerCompositor::needsFixedRootBackgroundLayer(const RenderLayer* layer) const
{
    if (layer != m_renderView->layer())
        return false;

    return supportsFixedRootBackgroundCompositing() && m_renderView->rootBackgroundIsEntirelyFixed();
}
