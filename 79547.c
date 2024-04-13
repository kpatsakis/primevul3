bool RenderLayerCompositor::requiresCompositingForOverflowScrollingParent(const RenderLayer* layer) const
{
    return !!layer->scrollParent();
}
