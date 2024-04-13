bool RenderLayerCompositor::needsContentsCompositingLayer(const RenderLayer* layer) const
{
    return layer->stackingNode()->hasNegativeZOrderList();
}
