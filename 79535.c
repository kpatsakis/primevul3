void RenderLayerCompositor::removeCompositedChildren(RenderLayer* layer)
{
    ASSERT(layer->hasCompositedLayerMapping());

    GraphicsLayer* hostingLayer = layer->compositedLayerMapping()->parentForSublayers();
    hostingLayer->removeAllChildren();
}
