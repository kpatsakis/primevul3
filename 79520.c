String RenderLayerCompositor::layerTreeAsText(LayerTreeFlags flags)
{
    updateCompositingLayers();

    if (!m_rootContentLayer)
        return String();

    GraphicsLayer* rootLayer = m_rootContentLayer.get();
    if (flags & LayerTreeIncludesRootLayer)
        rootLayer = rootGraphicsLayer();

    String layerTreeText = rootLayer->layerTreeAsText(flags);

    if (flags & LayerTreeIncludesRepaintRects)
        return m_renderView->frameView()->trackedRepaintRectsAsText() + layerTreeText;

    return layerTreeText;
}
