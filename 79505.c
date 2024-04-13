GraphicsLayer* RenderLayerCompositor::fixedRootBackgroundLayer() const
{
    RenderLayer* viewLayer = m_renderView->layer();
    if (!viewLayer)
        return 0;

    if (viewLayer->compositingState() == PaintsIntoOwnBacking && viewLayer->compositedLayerMapping()->backgroundLayerPaintsFixedRootBackground())
        return viewLayer->compositedLayerMapping()->backgroundLayer();

    return 0;
}
