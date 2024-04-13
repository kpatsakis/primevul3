void RenderLayerCompositor::updateViewportConstraintStatus(RenderLayer* layer)
{
    if (isViewportConstrainedFixedOrStickyLayer(layer))
        addViewportConstrainedLayer(layer);
    else
        removeViewportConstrainedLayer(layer);
}
