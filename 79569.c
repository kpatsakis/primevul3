void RenderLayerCompositor::setTracksRepaints(bool tracksRepaints)
{
    updateCompositingLayers();
    m_isTrackingRepaints = tracksRepaints;
}
