void RenderLayerCompositor::setCompositingParent(RenderLayer* childLayer, RenderLayer* parentLayer)
{
    ASSERT(!parentLayer || childLayer->ancestorCompositingLayer() == parentLayer);
    ASSERT(childLayer->hasCompositedLayerMapping());

    if (!parentLayer || !parentLayer->hasCompositedLayerMapping())
        return;

    if (parentLayer) {
        GraphicsLayer* hostingLayer = parentLayer->compositedLayerMapping()->parentForSublayers();
        GraphicsLayer* hostedLayer = childLayer->compositedLayerMapping()->childForSuperlayers();

        hostingLayer->addChild(hostedLayer);
    } else {
        childLayer->compositedLayerMapping()->childForSuperlayers()->removeFromParent();
    }
}
