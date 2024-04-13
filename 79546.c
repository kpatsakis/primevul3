bool RenderLayerCompositor::requiresCompositingForOverflowScrolling(const RenderLayer* layer) const
{
    return layer->needsCompositedScrolling();
}
