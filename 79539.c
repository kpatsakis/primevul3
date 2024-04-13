void RenderLayerCompositor::repaintOnCompositingChange(RenderLayer* layer)
{
    if (layer->renderer() != m_renderView && !layer->renderer()->parent())
        return;

    RenderLayerModelObject* repaintContainer = layer->renderer()->containerForRepaint();
    if (!repaintContainer)
        repaintContainer = m_renderView;

    layer->repainter().repaintIncludingNonCompositingDescendants(repaintContainer);
}
